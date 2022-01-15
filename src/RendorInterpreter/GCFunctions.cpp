#include "RendorInterpreter/Interpreter.hpp"

/* -------------------------------------------------------------------------- */
/*                          Memory related functions                          */
/* -------------------------------------------------------------------------- */
/* ---------------------------- Garbage Collector --------------------------- */
void Interpreter::GarbageCollector()
{
    WhiteObjects.clear();
    for (auto Constant : GreyObjects)
    {
        BlackObjects.push_back(std::move(Constant));
        break;
    }

    /* ------------------------ Remove all empty pointers ----------------------- */
    if (Objects.size())
    {
        Objects.erase(std::remove_if(Objects.begin(), Objects.end(), 
        [](TypeObjectPtr Ptr)
        {
            return Ptr.expired() || Ptr.lock()->ColorOfObj == GCColor::WHITE;
        }));
    }
    WhiteObjects.clear();
}

/* ------------------------ Function to add constants ----------------------- */
TypeObject Interpreter::CreateConstant(std::string_view Constant)
{
    if (Constant[0] == '_')
    {
        std::string ActualConstant{Constant.begin() + 2, Constant.end()};
        switch (Constant[1])
        {
            /* --------------------------------- Strings -------------------------------- */
            case 'S':
            {
                auto FindIterator = std::find_if(
                Objects.begin(), 
                Objects.end(),
                [&ActualConstant] (TypeObjectPtr Ptr) 
                {
                    if (!Ptr.expired())
                    {
                        return Ptr.lock()->m_Value == ActualConstant;
                    } 
                    else 
                    {
                        return false;
                    }
                }
                );
                
                // Add new string to Objects
                if (FindIterator == Objects.end())
                {
                    TypeObject Obj = std::make_shared<String>(std::string{ActualConstant});
                    WhiteObjects.push_back(Obj);
                    Objects.push_back(Obj);
                    return WhiteObjects.back();
                } 
                else 
                {
                    return FindIterator->lock();
                }
                break;
            }

            /* ---------------------------- Copying Variables --------------------------- */
            case '&':
            {
                if (GlobalVariables->contains(ActualConstant))
                {
                    return (*GlobalVariables)[ActualConstant]->m_ValueClass;
                }
                else if (CurrentScopeVariables->contains(ActualConstant))
                {
                    return (*CurrentScopeVariables)[ActualConstant]->m_ValueClass; 
                }
                else 
                {
                    throw error::RendorException("Reference to undefined variable: " + ActualConstant);
                }
                break;
            }

            default:
            {
                throw error::RendorException("Unsupported Constant Declared");
            }
        }
    }

    else 
    {
        auto FindIterator = std::find_if(
        Objects.begin(), 
        Objects.end(),
        [&Constant] (TypeObjectPtr Ptr) 
        {
            if (!Ptr.expired())
            {
                return Ptr.lock()->m_Value == Constant;
            } 
            else 
            {
                return false;
            }
        }
        );

        /* -------------------------- If item doesn't exist ------------------------- */
        if (FindIterator == Objects.end())
        {
            /* ----------------------------- Ints and floats ---------------------------- */
            if 
            (Constant.find_first_not_of("1234567890") == std::string::npos) 
            {
                if (Constant.find_first_of(".") != std::string::npos)
                {
                    TypeObject Obj = std::make_shared<Float>(std::string{Constant});
                    WhiteObjects.push_back(Obj);
                    Objects.push_back(Obj);
                }
                else 
                {
                    TypeObject Obj = std::make_shared<Int>(std::string{Constant});
                    WhiteObjects.push_back(Obj);
                    Objects.push_back(Obj);
                }
            }
            
            /* -------------------------------- Booleans -------------------------------- */
            else if 
            ((Constant == "true") ||
            (Constant == "false"))
            {
                TypeObject Obj = std::make_shared<Bool>(std::string{Constant});
                WhiteObjects.push_back(Obj);
                Objects.push_back(Obj);
            }

            /* ------------------------------ return result ----------------------------- */
            return WhiteObjects.back();
        }
        else 
        {
            return FindIterator->lock();
        }
    }

    return TypeObject();
}

void Interpreter::MarkConstantBlack(TypeObject Const)
{
    switch (Const->ColorOfObj)
    {
        case GCColor::BLACK:
        {
            break;
        }

        case GCColor::GREY:
        {
            auto OriginalPtr = std::find(GreyObjects.begin(), GreyObjects.end(), Const);
            BlackObjects.push_back(std::move(*OriginalPtr));
            break;
        }

        default:
        {
            auto OriginalPtr = std::find(WhiteObjects.begin(), WhiteObjects.end(), Const);
            BlackObjects.push_back(std::move(*OriginalPtr));
            break;
        }
    }
    Const->ColorOfObj = GCColor::BLACK;
}


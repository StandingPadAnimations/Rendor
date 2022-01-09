#include "RendorInterpreter/Interpreter.hpp"

/* -------------------------------------------------------------------------- */
/*                          Memory related functions                          */
/* -------------------------------------------------------------------------- */
/* ---------------------------- Garbage Collector --------------------------- */
void Interpreter::GarbageCollector()
{
    // Deleting variables
    for (auto const& Var : DisposedVariables)
    {
        TypeObjectPtr VariableValuePtr((*CurrentScopeVariables)[std::string{Var}]->m_ValueClass);

        if (SharedVariables.contains(VariableValuePtr->m_ID))
        {
            SharedVariables[VariableValuePtr->m_ID].pop_back();

            // If there's only one variable using that object
            if (SharedVariables[VariableValuePtr->m_ID].size() == 1)
            {
                SharedVariables.erase(VariableValuePtr->m_ID);
            }
        } 
        
        else 
        {
            auto FindIterator = std::find_if(
                BlackObjects.begin(), 
                BlackObjects.end(),
                [&VariableValuePtr] (const TypeObject& Ptr) 
                {
                    return Ptr.get()->m_ID == VariableValuePtr->m_ID;
                }
                );

            (*VariableValuePtr).ColorOfObj = GCColor::WHITE; // Set it as white
            WhiteObjects.push_back(std::move(*FindIterator));
        }
    }

    // Removing constants
    WhiteObjects.clear();
    Objects.erase(std::remove_if(Objects.begin(), Objects.end(), 
        [](TypeObjectPtr Obj) 
        { 
            return Obj.IsValid() == false; 
        }), 
        Objects.end());

    std::sort(BlackObjects.begin(), BlackObjects.end(), CompareRendorIDs);
}

/* ------------------------ Function to add constants ----------------------- */
TypeObjectPtr Interpreter::CreateConstant(std::string_view Constant)
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
                    return Ptr->m_Value == ActualConstant;
                }
                );
                
                // Add new string to Objects
                if (FindIterator == Objects.end())
                {
                    WhiteObjects.push_back(std::make_unique<String>(ActualConstant)); // All new objects are white by default
                    return TypeObjectPtr(WhiteObjects.back());
                } 
                else 
                {
                    return TypeObjectPtr(*FindIterator);
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
                    throw error::RendorException("Reference to undefined variable");
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
        [&Constant] (TypeObjectPtr& Ptr) 
        {
            return Ptr->m_Value == Constant;
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
                    WhiteObjects.push_back(std::make_unique<Float>(std::string{Constant}));
                    Objects.push_back(TypeObjectPtr(WhiteObjects.back()));
                }
                else 
                {
                    WhiteObjects.push_back(std::make_unique<Int>(std::string{Constant}));
                    Objects.push_back(TypeObjectPtr(WhiteObjects.back()));
                }
            }
            
            /* -------------------------------- Booleans -------------------------------- */
            else if 
            ((Constant == "true") ||
            (Constant == "false"))
            {
                WhiteObjects.push_back(std::make_unique<Bool>(std::string{Constant}));
                Objects.push_back(TypeObjectPtr(WhiteObjects.back()));
            }

            /* ------------------------------ return result ----------------------------- */
            return Objects.back();
        }
        else 
        {
            return TypeObjectPtr(*FindIterator);
        }
    }
}

void Interpreter::MarkConstantBlack(TypeObjectPtr& Const)
{
    /* ---------------------- if object isn't marked black ---------------------- */
    if (Const->ColorOfObj != GCColor::BLACK)
    {
        switch (Const->ColorOfObj)
        {
            case GCColor::WHITE:
            {
                auto WhiteFindIterator = std::find_if(
                    WhiteObjects.begin(), 
                    WhiteObjects.end(),
                    [&Const] (TypeObject& Ptr) 
                    {
                        return Ptr.get()->m_ID == Const->m_ID;
                    }
                    );
                BlackObjects.push_back(std::move(*WhiteFindIterator));
                break;
            }

            case GCColor::GREY:
            {
                auto GreyFindIterator = std::find_if(
                    GreyObjects.begin(), 
                    GreyObjects.end(),
                    [&Const] (TypeObject& Ptr) 
                    {
                        return Ptr.get()->m_ID == Const->m_ID;
                    }
                    );
                BlackObjects.push_back(std::move(*GreyFindIterator));
                break;
            }

            case GCColor::BLACK:
            {
                // If this error pops, then the compiler has a bug
                throw error::RendorException("what the frick, how did this happen?");
            }
        }

        /* --------------------------------- Mark it -------------------------------- */
        if (Const.IsValid())
        {
            Const->ColorOfObj = GCColor::BLACK;
        }
        else 
        {
            throw error::RendorException("what the frick, how did this happen?");
        }
    }
}
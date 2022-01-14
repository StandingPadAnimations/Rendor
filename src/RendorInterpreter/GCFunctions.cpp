#include "RendorInterpreter/Interpreter.hpp"

/* -------------------------------------------------------------------------- */
/*                          Memory related functions                          */
/* -------------------------------------------------------------------------- */
/* ---------------------------- Garbage Collector --------------------------- */
void Interpreter::GarbageCollector()
{
    for (auto Ptr = Objects.begin(); Ptr != Objects.end();)
    {
        if (std::find(WhiteObjects.begin(), WhiteObjects.end(), (*Ptr)->m_ID) != WhiteObjects.end())
        {
            Objects.erase(Ptr);
        }
        else 
        {
            ++Ptr;
        }
    }
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
                    TypeObject Obj = std::make_unique<String>(std::string{ActualConstant});
                    WhiteObjects.push_back(Obj->m_ID);
                    Objects.push_back(std::move(Obj));
                    return TypeObjectPtr(Objects.back());
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
        [&Constant] (TypeObject& Ptr) 
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
                    TypeObject Obj = std::make_unique<Float>(std::string{Constant});
                    WhiteObjects.push_back(Obj->m_ID);
                    Objects.push_back(std::move(Obj));
                }
                else 
                {
                    TypeObject Obj = std::make_unique<Int>(std::string{Constant});
                    WhiteObjects.push_back(Obj->m_ID);
                    Objects.push_back(std::move(Obj));
                }
            }
            
            /* -------------------------------- Booleans -------------------------------- */
            else if 
            ((Constant == "true") ||
            (Constant == "false"))
            {
                TypeObject Obj = std::make_unique<Bool>(std::string{Constant});
                WhiteObjects.push_back(Obj->m_ID);
                Objects.push_back(std::move(Obj));
            }

            /* ------------------------------ return result ----------------------------- */
            return TypeObjectPtr(Objects.back());
        }
        else 
        {
            return TypeObjectPtr(*FindIterator);
        }
    }

    return TypeObjectPtr();
}

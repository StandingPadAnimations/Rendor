#include "RendorInterpreter/Interpreter.hpp"

TypeTuple Interpreter::ParseConstant(std::string_view Constant)
{
    switch (Constant[0])
    {
        case '(':
        {
            size_t Comma = Constant.find_first_of(",");
            std::string_view Const1 {Constant.begin() + 1, (Comma - 1)};
            std::string_view Const2 {(Constant.begin() + 1) + Comma, (Constant.end() - 1)};;
            return TypeTuple(CreateConstant(Const1), CreateConstant(Const2));
        }
        default:
        {
            return TypeTuple(CreateConstant(Constant), TypeObjectPtr());
            break;
        }
    }
}

/* ------------------------ Function to add constants ----------------------- */
TypeObject Interpreter::CreateConstant(std::string_view Constant)
{
    if (Constant[0] == '_')
    {
        std::string ActualConstant{Constant.begin() + 2, Constant.end()};

        if (Constant.size() <= 2)
        {
            ActualConstant = "";
        }

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

    else if (Constant[0] == '&')
    {
        std::string ActualConstant{Constant.begin() + 3, Constant.end()};
        if (Constant[2] == 'A')
        {
            return CreateConstant(PostFixEval(ActualConstant));
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
            (Constant.find_first_not_of("1234567890.") == std::string::npos) 
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

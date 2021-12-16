#include "RendorInterpreter/RendorTypes.hpp"

/* -------------------------------------------------------------------------- */
/*                                     Int                                    */
/* -------------------------------------------------------------------------- */
bool Int::IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator)
{
    std::variant<int64_t, double, bool> Const2Converted; // All compatible types with Int
    int64_t Const1Converted = RetriveVariable();

    /* ------------------------- Retrive value of Const2 ------------------------ */
    if (Const2->TypeOfVariable() == VariableType::Int)
    {
        auto& Const2Object = static_cast<Int&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }
    else if (Const2->TypeOfVariable() == VariableType::Float)
    {
        auto& Const2Object = static_cast<Float&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }
    else if (Const2->TypeOfVariable() == VariableType::Bool)
    {
        auto& Const2Object = static_cast<Bool&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }

    /* -------------------------------- Evaluate -------------------------------- */
    switch (ComparisonOperator)
    {
        case Operator::EQUAL:
        {
            if 
            (((Const1Converted == std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  == std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))) ||
            ((Const1Converted  == std::holds_alternative<bool>(Const2Converted))    && (std::get<bool>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::NOT_EQUAL:
        {
            if 
            (((Const1Converted != std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  != std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))) ||
            ((Const1Converted  != std::holds_alternative<bool>(Const2Converted))    && (std::get<bool>(Const2Converted))))
            {
            {
                return true;
            }
            break;
        }

        case Operator::GREATER_THAN:
        {
            if 
            (((Const1Converted > std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  > std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::LESS_THAN:
        {
            if 
            (((Const1Converted < std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  < std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::GREATER_OR_EQUAL:
        {
            if 
            (((Const1Converted >= std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  >= std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::LESS_OR_EQUAL:
        {
            if 
            (((Const1Converted <= std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  <= std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }
    }
    }
    /* --------------------------- If it was not valid -------------------------- */
    return false; 
}

/* -------------------------------------------------------------------------- */
/*                                    Float                                   */
/* -------------------------------------------------------------------------- */

bool Float::IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator)
{
    std::variant<int64_t, double, bool> Const2Converted; // All compatible types with Float
    double Const1Converted = RetriveVariable();

    /* ------------------------- Retrive value of Const2 ------------------------ */
    if (Const2->TypeOfVariable() == VariableType::Int)
    {
        auto& Const2Object = static_cast<Int&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }
    else if (Const2->TypeOfVariable() == VariableType::Float)
    {
        auto& Const2Object = static_cast<Float&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }
    else if (Const2->TypeOfVariable() == VariableType::Bool)
    {
        auto& Const2Object = static_cast<Bool&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }

    /* -------------------------------- Evaluate -------------------------------- */
    switch (ComparisonOperator)
    {
        case Operator::EQUAL:
        {
            if 
            (((Const1Converted == std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  == std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))) ||
            ((Const1Converted  == std::holds_alternative<bool>(Const2Converted))    && (std::get<bool>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::NOT_EQUAL:
        {
            if 
            (((Const1Converted != std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  != std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::GREATER_THAN:
        {
            if 
            (((Const1Converted > std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  > std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::LESS_THAN:
        {
            if 
            (((Const1Converted < std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  < std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::GREATER_OR_EQUAL:
        {
            if 
            (((Const1Converted >= std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  >= std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::LESS_OR_EQUAL:
        {
            if 
            (((Const1Converted <= std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  <= std::holds_alternative<double>(Const2Converted))  && (std::get<double>(Const2Converted))))
            {
                return true;
            }
            break;
        }
    }

    /* --------------------------- If it was not valid -------------------------- */
    return false; 
}

/* -------------------------------------------------------------------------- */
/*                                   String                                   */
/* -------------------------------------------------------------------------- */
bool String::IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator)
{
    std::string Const2Converted; // String is only compatible with string
    std::string Const1Converted = RetriveVariable();

    /* ------------------------- Retrive value of Const2 ------------------------ */
    if (Const2->TypeOfVariable() == VariableType::String)
    {
        auto& Const2Object = static_cast<String&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }

    /* -------------------------------- Evaluate -------------------------------- */
    switch (ComparisonOperator)
    {
        case Operator::EQUAL:
        {
            if (Const1Converted == Const2Converted)
            {
                return true;
            }
            break;
        }

        case Operator::NOT_EQUAL:
        {
            if (Const1Converted != Const2Converted)
            {
                return true;
            }
            break;
        }

        default:
        {
            throw error::RendorException("Strings can only be compared by == and !=");
        }
    }

    /* --------------------------- If it was not valid -------------------------- */
    return false; 
}


/* -------------------------------------------------------------------------- */
/*                                    Bool                                    */
/* -------------------------------------------------------------------------- */
bool Bool::IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator)
{
    std::variant<int64_t, double, bool> Const2Converted; // All compatible types with Float
    double Const1Converted = RetriveVariable();

    /* ------------------------- Retrive value of Const2 ------------------------ */
    if (Const2->TypeOfVariable() == VariableType::Int)
    {
        auto& Const2Object = static_cast<Int&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }
    else if (Const2->TypeOfVariable() == VariableType::Float)
    {
        auto& Const2Object = static_cast<Float&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }
    else if (Const2->TypeOfVariable() == VariableType::Bool)
    {
        auto& Const2Object = static_cast<Bool&>(*Const2);
        Const2Converted = Const2Object.RetriveVariable();
    }

    /* -------------------------------- Evaluate -------------------------------- */
    switch (ComparisonOperator)
    {
        case Operator::EQUAL:
        {
            if 
            (((Const1Converted == std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  == std::holds_alternative<double>(Const2Converted)) && (std::get<double>(Const2Converted))) ||
            ((Const1Converted  == std::holds_alternative<bool>(Const2Converted))   && (std::get<bool>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        case Operator::NOT_EQUAL:
        {
            if 
            (((Const1Converted != std::holds_alternative<int64_t>(Const2Converted)) && (std::get<int64_t>(Const2Converted))) ||
            ((Const1Converted  != std::holds_alternative<double>(Const2Converted)) && (std::get<double>(Const2Converted))) ||
            ((Const1Converted  != std::holds_alternative<bool>(Const2Converted))   && (std::get<bool>(Const2Converted))))
            {
                return true;
            }
            break;
        }

        default:
        {
            throw error::RendorException("Bools can only be compared by == and !=");
        }
    }

    /* --------------------------- If it was not valid -------------------------- */
    return false; 
}

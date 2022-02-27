#include "RendorInterpreter/Resources/RendorTypes.hpp"

/* -------------------------------------------------------------------------- */
/*                                     Int                                    */
/* -------------------------------------------------------------------------- */
bool Int::IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator)
{
    std::variant<int64_t, double, bool> Const2Converted; // All compatible types with Int

    /* ------------------------- Retrive value of Const2 ------------------------ */
    if (Const2->TypeOfVariable() == VariableType::Int64)
    {
        auto& Const2Object = static_cast<Int&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }
    else if (Const2->TypeOfVariable() == VariableType::Double)
    {
        auto& Const2Object = static_cast<Float&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }
    else if (Const2->TypeOfVariable() == VariableType::Bool)
    {
        auto& Const2Object = static_cast<Bool&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }

    /* -------------------------------- Evaluate -------------------------------- */
    switch (ComparisonOperator)
    {
        case Operator::EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue == v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::NOT_EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue != v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::GREATER_THAN:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue > v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::LESS_THAN:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue < v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::GREATER_OR_EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue >= v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::LESS_OR_EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue <= v; 
            }, Const2Converted))
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
/*                                    Float                                   */
/* -------------------------------------------------------------------------- */

bool Float::IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator)
{
    std::variant<int64_t, double, bool> Const2Converted; // All compatible types with Float

    /* ------------------------- Retrive value of Const2 ------------------------ */
    if (Const2->TypeOfVariable() == VariableType::Int64)
    {
        auto& Const2Object = static_cast<Int&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }
    else if (Const2->TypeOfVariable() == VariableType::Double)
    {
        auto& Const2Object = static_cast<Float&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }
    else if (Const2->TypeOfVariable() == VariableType::Bool)
    {
        auto& Const2Object = static_cast<Bool&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }

    /* -------------------------------- Evaluate -------------------------------- */
    switch (ComparisonOperator)
    {
        case Operator::EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue == v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::NOT_EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue != v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::GREATER_THAN:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue > v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::LESS_THAN:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue < v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::GREATER_OR_EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue >= v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::LESS_OR_EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue <= v; 
            }, Const2Converted))
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
bool String::IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator)
{
    std::string Const2Converted; // String is only compatible with string
    std::string ConvertedValue = m_Value;

    /* ------------------------- Retrive value of Const2 ------------------------ */
    if (Const2->TypeOfVariable() == VariableType::String)
    {
        auto& Const2Object = static_cast<String&>(*Const2);
        Const2Converted = Const2Object.m_Value;
    }

    /* -------------------------------- Evaluate -------------------------------- */
    switch (ComparisonOperator)
    {
        case Operator::EQUAL:
        {
            if (ConvertedValue == Const2Converted)
            {
                return true;
            }
            break;
        }

        case Operator::NOT_EQUAL:
        {
            if (ConvertedValue != Const2Converted)
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
bool Bool::IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator)
{
    std::variant<int64_t, double, bool> Const2Converted; // All compatible types with Float

    /* ------------------------- Retrive value of Const2 ------------------------ */
    if (Const2->TypeOfVariable() == VariableType::Int64)
    {
        auto& Const2Object = static_cast<Int&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }
    else if (Const2->TypeOfVariable() == VariableType::Double)
    {
        auto& Const2Object = static_cast<Float&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }
    else if (Const2->TypeOfVariable() == VariableType::Bool)
    {
        auto& Const2Object = static_cast<Bool&>(*Const2);
        Const2Converted = Const2Object.ConvertedValue;
    }

    /* -------------------------------- Evaluate -------------------------------- */
    switch (ComparisonOperator)
    {
        case Operator::EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue == v; 
            }, Const2Converted))
            {
                return true;
            }
            break;
        }

        case Operator::NOT_EQUAL:
        {
            if 
            (std::visit(
            [&](auto v) 
            { 
                return ConvertedValue != v; 
            }, Const2Converted))
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

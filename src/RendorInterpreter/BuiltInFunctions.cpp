#include "RendorInterpreter/Interpreter.hpp"

/* -------------------------------------------------------------------------- */
/*                             Built In Functions                             */
/* -------------------------------------------------------------------------- */

std::optional<TypeObjectPtr> Interpreter::RENDOR_ECHO_FUNCTION(std::vector<TypeObjectPtr>& EchoArgs)
{
    // Newline
    if (EchoArgs[0].IsValid()){}
    else
    {
        throw error::RendorException("Reference to null const");
    }
    std::cout << EchoArgs[0]->m_Value << '\n';
    return {};
}

std::optional<TypeObjectPtr> Interpreter::RENDOR_INPUT_FUNCTION(std::vector<TypeObjectPtr>& InputArgs)
{
    std::string InputString;
    
    if (InputArgs[0].IsValid()){}
    else 
    {
        throw error::RendorException("Reference to null const");
    }

    std::cout << InputArgs[0]->m_Value;
    std::getline(std::cin, InputString);
    return CreateConstant("_S" + InputString);
}

std::optional<TypeObjectPtr> Interpreter::RENDOR_SUM_FUNCTION(std::vector<TypeObjectPtr>& SumArgs)
{
    if (SumArgs[0].IsValid()){
        if (SumArgs[0]->TypeOfVariable() != VariableType::Int)
        {
            throw error::RendorException("Sum Function error: Argument must be int");
        }
    }
    else 
    {
        throw error::RendorException("Reference to null const");
    }
    
    auto& NumberToSum = static_cast<Int&>(*SumArgs[0]);
    int64_t SumNumber = NumberToSum.ConvertedValue;

    SumNumber = (SumNumber *  (SumNumber + 1)) / 2;
    return CreateConstant(std::to_string(SumNumber));
}
#include "RendorInterpreter/Interpreter.hpp"
#include "RendorInterpreter/MathEval.hpp"


std::string Interpreter::PostFixEval(std::string_view PostFixOperation)
{
    boost::char_separator<char> Delimeters(" ", "");
    boost::tokenizer<boost::char_separator<char>> SeparatedOperation(PostFixOperation, Delimeters);

    std::list<TypeObjectPtr> Stack;

    for(auto const& PartOfOperation : SeparatedOperation){
        if (
        (PartOfOperation.find_first_of("^*/+-") != std::string::npos) && 
        (PartOfOperation.find_first_of("1234567890.") == std::string::npos)) // Just in case a number and operator are together
        {
            MathVariant Num1;
            MathVariant Num2;
            SetVariant<MathVariant>(Num2, Stack.back());
            Stack.pop_back();
            SetVariant<MathVariant>(Num1, Stack.back());
            Stack.pop_back();
            Stack.emplace_back(CreateConstant(EvalNums(Num1, Num2, PartOfOperation)));
        } 
        else
        {
            if ( // variables in math operations 
            (PartOfOperation[0] == '_') &&
            (PartOfOperation[1] == '&'))
            {
                const auto CopiedVariableName = PartOfOperation.substr(2, PartOfOperation.size()-2);
                const auto CopiedVariable = GetConstFromVariable(CopiedVariableName);

                if ( // confirm if variable can be operated on 
                (CopiedVariable.lock()->TypeOfVariable() != VariableType::Int) &&
                (CopiedVariable.lock()->TypeOfVariable() != VariableType::Float)
                )
                {
                    throw error::RendorException((boost::format("%s is not an int or float!") % CopiedVariableName).str());
                }
                Stack.emplace_back(CopiedVariable);
            } 
            else
            {
                Stack.emplace_back(CreateConstant(PartOfOperation));
            }
        }
    }
    if (Stack.size() > 1)
    {
        throw error::RendorException("WTH Error; Size of stack in arithmethic operation is larger then expected. Please report this as an issue on the Rendor Github.");
    } 

    std::string& FinalOutput = Stack.back().lock()->m_Value;
    return FinalOutput;
}

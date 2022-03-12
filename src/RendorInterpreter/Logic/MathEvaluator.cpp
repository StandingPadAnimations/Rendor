#include "RendorInterpreter/Interpreter.hpp"
#include "RendorInterpreter/MathEval.hpp"


void Interpreter::PostFixEval(std::string_view PostFixOperation)
{
    boost::char_separator<char> Delimeters(" ", "");
    boost::tokenizer<boost::char_separator<char>> SeparatedOperation(PostFixOperation, Delimeters);

    std::list<MathVariant> Stack;

    for(auto const& PartOfOperation : SeparatedOperation){
        if (
        (PartOfOperation.find_first_of("^*/+-") != std::string::npos) && 
        (PartOfOperation.find_first_of("1234567890.") == std::string::npos)) // Just in case a number and operator are together
        {
            /* ------------------------------- set values ------------------------------- */
            MathVariant Num1 = Stack.back();
            Stack.pop_back();
            MathVariant Num2= Stack.back();
            Stack.pop_back();

            /* ------------------------------- Grab result ------------------------------ */
            Stack.emplace_back(EvalNums(Num1, Num2, PartOfOperation));
        } 
        else
        {
            if ( // variables in math operations 
            (PartOfOperation[0] == '_') &&
            (PartOfOperation[1] == '&'))
            {
                /* -------------------------- Add value of variable ------------------------- */
                const auto CopiedVariableName = PartOfOperation.substr(2, PartOfOperation.size()-2);
                GetConstFromVariable(CopiedVariableName);
            } 
            else
            {
                CreateConstant(PartOfOperation);
            }


            /* -------------------------------- Get type -------------------------------- */
            switch (GrabObjTypeFromStack())
            {
                case VariableType::Int64:
                {
                    Stack.emplace_back(GrabInt64FromStack());
                    break;
                }
                case VariableType::Double:
                {
                    Stack.emplace_back(GrabDoubleFromStack());
                    break;
                }
                default:
                {
                    break;
                }
            }

            /* -------------------------------- Pop stack ------------------------------- */
            PopStack();
        }
    }
    if (Stack.size() > 1)
    {
        throw error::RendorException("WTH Error; Size of stack in arithmethic operation is larger then expected. Please report this as an issue on the Rendor Github.");
    }
}

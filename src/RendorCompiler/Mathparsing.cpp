#include "RendorCompiler/Mathparsing.hpp"

// TODO: finish this lol
std::string OperationToPostfix (std::string_view Operation){
    std::list<std::string> Stack; // ? Perhaps a list would be better
    std::map<std::string, int> OperatorPresedence {{"^", 3}, {"*", 2}, {"/", 2}, {"+", 1}, {"-", 1}}; // based on PEMDAS 
    std::string Output = "";
    bool LastTokenWasOperator = false;

    boost::char_separator<char> Delimeters("", "^*/+-()");
    boost::tokenizer<boost::char_separator<char>> SeparatedOperation(Operation, Delimeters);

    for (auto const& PartOfOperation : SeparatedOperation){
        uint8_t CurrentOp = OperatorPresedence[PartOfOperation];
        uint8_t StackOp = 0;

        if (Stack.size())
        {
            StackOp = OperatorPresedence[Stack.back()];
        }

        if (
        (PartOfOperation.find_first_of("^*/+-") != std::string::npos) &&
        (PartOfOperation.find_first_of("1234567890.") == std::string::npos)) // Just in case a number and operator are together
        {
            if ( 
            (Stack.empty()) ||
            (Stack.back() == "(") ||
            (CurrentOp > StackOp))
            {
                Stack.emplace_back(PartOfOperation); // Places the operator in the stack if it has a high presedence then the previous
            }

            else if (CurrentOp <= StackOp) // Remove operator on top of the stack, put it in output, and add new operator
            { 
                do
                {
                    Output += " " + Stack.back();
                    Stack.pop_back();
                    if (Stack.empty())
                    {
                        break;
                    }
                } while (CurrentOp <= StackOp);
                Stack.emplace_back(PartOfOperation);
            }
            
            else
            {
                throw error::RendorException("WTH error; Invalid binary operator");
            }
            LastTokenWasOperator = true;
        } 

        else if(
        (PartOfOperation.find_first_of("(") != std::string::npos) &&
        (PartOfOperation.find_first_of("1234567890.") == std::string::npos)) // when it encounters )
        {
            if(!LastTokenWasOperator){ // add a * to the stack
                if (!Stack.empty())
                {
                    if (
                    (OperatorPresedence["*"] > OperatorPresedence[Stack.back()]) &&
                    (!Output.empty()))
                    {
                        Stack.emplace_back("*"); // Places the operator in the stack if it has a high presedence then the previous
                    }

                    else if (OperatorPresedence["*"] <= OperatorPresedence[Stack.back()]) // Remove operator on top of the stack, put it in output, and add new operator
                    { 
                        Output += " " + Stack.back();
                        Stack.pop_back();
                        Stack.emplace_back("*");
                    }
                    else
                    {
                        // Skip if there's nothing in output
                    }
                }
                else
                {
                    Stack.emplace_back("*"); 
                }
            }
            Stack.emplace_back(PartOfOperation);
        }

        else if(
        (PartOfOperation.find_first_of(")") != std::string::npos) &&
        (PartOfOperation.find_first_of("1234567890.") == std::string::npos)) // when it encounters )
        {
            while (Stack.back() != "(")
            {
                Output += " " + Stack.back();
                Stack.pop_back();
            }

            if (!Stack.empty())
            {
                Stack.pop_back(); 
            }
        }

        else
        {
            if (Output.back() != ' ')
            {
                Output += " " + PartOfOperation;
            }
            else 
            {
                Output += PartOfOperation;
            }
            LastTokenWasOperator = false;
        }
    }
    for (std::list<std::string>::reverse_iterator Operation = Stack.rbegin(); Operation != Stack.rend(); ++Operation) // Add any leftover operators
    { 
        if (Output.back() != ' ')
        {
            Output += " " + (*Operation);
        }
        else 
        {
            Output += (*Operation);
        }
    }
    return Output;
}
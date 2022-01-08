#include "RendorCompiler/Mathparsing.hpp"

// TODO: finish this lol
std::string OperationToPostfix (std::string_view Operation){
    std::list<std::string> Stack; // ? Perhaps a list would be better
    std::map<std::string, int> OperatorPresedence {{"^", 3}, {"*", 2}, {"/", 2}, {"+", 1}, {"-", 1}}; // based on PEMDAS 
    std::string Output = "";
    bool LastTokenWasOperator = false;

    // Does the actual seperation 
    boost::char_separator<char> Delimeters("", "^*/+-");
    boost::tokenizer<boost::char_separator<char>> SeparatedOperation(Operation, Delimeters);

    for (auto const& PartOfOperation : SeparatedOperation){
        if (
        (PartOfOperation.find_first_of("^*/+-") != std::string::npos) &&
        (PartOfOperation.find_first_of("1234567890.") == std::string::npos)) // Just in case a number and operator are together
        {
            if ( 
            (Stack.size() == 0) ||
            (Stack.back() == "(") ||
            (OperatorPresedence[PartOfOperation] > OperatorPresedence[Stack.back()]))
            {
                Stack.emplace_back(PartOfOperation); // Places the operator in the stack if it has a high presedence then the previous
            }

            else if (OperatorPresedence[PartOfOperation] <= OperatorPresedence[Stack.back()]) // Remove operator on top of the stack, put it in output, and add new operator
            { 
                Output += " " + Stack.back();
                Stack.pop_back();
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
                if (
                (OperatorPresedence["*"] > OperatorPresedence[Stack.back()]) &&
                (Output.size() > 0))
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
            Stack.pop_back(); 
        }

        else
        {
            if (Output.size() == 0)
            {
                Output += PartOfOperation;
            } 
            else{
                Output += " " + PartOfOperation;
            }
            LastTokenWasOperator = false;
        }
    }
    for (std::list<std::string>::reverse_iterator Operation = Stack.rbegin(); Operation != Stack.rend(); ++Operation) // Add any leftover operators
    { 
        Output += " " + (*Operation);
    }
    return Output;
}
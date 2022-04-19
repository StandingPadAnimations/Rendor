#include "RendorCompiler/Math/Mathparsing.hpp"

std::string OperationToPostfix(std::string_view Operation){
    std::vector<std::string> Stack;
    Stack.reserve(10);
    const std::map<std::string_view, int> OperatorPresedence {{"^", 3}, {"*", 2}, {"/", 2}, {"+", 1}, {"-", 1}}; // based on PEMDAS 

    std::string Output = "";
    bool LastTokenWasOperator = false;
    boost::char_separator<char> Delimeters("", "^*/+-()");
    boost::tokenizer<boost::char_separator<char>> SeparatedOperation(Operation, Delimeters);

    for (const auto& PartOfOperation : SeparatedOperation)
    {
        // if we are on an operator
        if (OperatorPresedence.contains(PartOfOperation))
        {
            // if the stack is empty, let's move on
            LastTokenWasOperator = true;
            if 
            ((Stack.empty()) ||
            (Stack.back() == "("))
            {
                Stack.emplace_back(PartOfOperation);
                continue; // Move on to the next operation
            }

            // Get the values of the operators for comparison
            uint32_t CurrentOp = OperatorPresedence.at(PartOfOperation);;
            uint32_t StackOp = OperatorPresedence.at(Stack.back());
            if (CurrentOp > StackOp)
            {
                Stack.emplace_back(PartOfOperation);
            }
            else
            {
                // Start adding the operators
                do
                {
                    Output += std::string{Stack.back()} + " ";
                    Stack.pop_back();
                    
                    if (Stack.empty())
                    {
                        break; // Break if the stack is empty
                    }
                    StackOp = OperatorPresedence.at(Stack.back());
                } while (StackOp >= CurrentOp);
            }
        }
        else 
        {
            if (PartOfOperation == "(") // ()
            {
                // If we need to add a * token
                if (!LastTokenWasOperator)
                {
                    // Repeat this code
                    if (Stack.empty())
                    {
                        Stack.emplace_back("*");
                        Stack.emplace_back(PartOfOperation);
                        continue; // Move on to the next operation
                    }

                    // Get the values of the operators for comparison
                    uint32_t CurrentOp = OperatorPresedence.at("*");;
                    uint32_t StackOp = OperatorPresedence.at(Stack.back());
                    if (CurrentOp > StackOp)
                    {
                        Stack.emplace_back("*");
                    }
                    else
                    {
                        // Start adding the operators
                        do
                        {
                            Output += std::string{Stack.back()} + " ";
                            Stack.pop_back();
                            
                            if (Stack.empty())
                            {
                                break; // Break if the stack is empty
                            }
                            StackOp = OperatorPresedence.at(Stack.back());
                        } while (StackOp >= CurrentOp);
                    }
                }
                Stack.emplace_back(PartOfOperation);
                LastTokenWasOperator = true;
            }
            else if (PartOfOperation == ")") // )
            {
                do 
                {
                    if (Stack.empty())
                    {
                        break;
                    }
                    Output += std::string{Stack.back()} + " ";
                    Stack.pop_back();
                } while (Stack.back() != "(");
                Stack.pop_back();
                LastTokenWasOperator = false;
            }
            else // Numeric types
            {
                // Add number to the output
                Output += PartOfOperation + " ";
                LastTokenWasOperator = false;
            }
        }
    }

    if (!Stack.empty())
    {
        for (std::vector<std::string>::reverse_iterator Op = Stack.rbegin(); Op != Stack.rend(); ++Op) // Add any leftover operators
        {
            if 
            ((*Op == "(") ||
            (*Op == ")"))
            {
                continue;
            }
            else
            {
                Output += (*Op) + " ";
            }
        }
    }
    return Output;
}
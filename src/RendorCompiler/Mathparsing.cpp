#include "RendorCompiler/Mathparsing.hpp"

// TODO: finish this lol
std::string OperationToPostfix(std::string_view Operation){
    std::vector<std::string> Stack; // ? Perhaps a list would be better
    std::map<std::string, int> OperatorPresedence {{"^", 3}, {"*", 2}, {"/", 2}, {"+", 1}, {"-", 1},};
    std::string Output = "";

    // Does the actual seperation 
    boost::char_separator<char> Delimeters("", "^*/+-");
    boost::tokenizer<boost::char_separator<char>> SeparatedOperation(Operation, Delimeters);

    for(auto const& PartOfOperation : SeparatedOperation){
        // ! This could be way easier to write and in a more readable way
        if(
        (PartOfOperation == "^") ||
        (PartOfOperation == "*") ||
        (PartOfOperation == "/") ||
        (PartOfOperation == "+") ||
        (PartOfOperation == "-")
        ){
            if(
            (Stack.size() == 0) ||
            (OperatorPresedence[PartOfOperation] > OperatorPresedence[Stack.back()])
            ){
                Stack.emplace_back(PartOfOperation); // Places the operator in the stack if it has a high presedence then the previous
            }
            else if((OperatorPresedence[PartOfOperation] <= OperatorPresedence[Stack.back()])){
                Output += " " + Stack.back();
                Stack.pop_back();
                Stack.emplace_back(PartOfOperation);
            }
            else {
                throw error::RendorException("WTH error; Invalid binary operator");
            }
        } else{
            if(Output.size() == 0){
                Output += PartOfOperation;
            } else{
                Output += " " + PartOfOperation;
            }
        }
    }
    for(auto const& Operation : Stack){ // Add any leftover operators
        Output += " " + Operation;
    }
    return Output;
}
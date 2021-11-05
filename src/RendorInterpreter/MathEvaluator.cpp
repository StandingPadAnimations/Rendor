#include "RendorInterpreter/MathEvaluator.hpp"

std::string EvalNums(std::string Num1, std::string Num2, std::string Op){
    std::string Answer;
    long double N1 = std::stold(Num1);
    long double N2 = std::stold(Num2);
    
    if(Op == "^"){
        Answer = std::to_string(pow(N1, N2));
    }

    else if(Op == "+"){
        Answer = std::to_string(N1+N2);
    }
            
    else if(Op == "-"){
        Answer = std::to_string(N1-N2);
    }

    else if(Op == "*"){
        Answer = std::to_string(N1*N2);
    }
            
    else if(Op == "/"){
        Answer = std::to_string(N1/N2);
    }
    return Answer;
}

std::string PostFixEval(std::string_view PostFixOperation){
    boost::char_separator<char> Delimeters(" ", "");
    boost::tokenizer<boost::char_separator<char>> SeparatedOperation(PostFixOperation, Delimeters);

    std::vector<std::string> Stack;

    for(auto const& PartOfOperation : SeparatedOperation){
        if(
        (PartOfOperation == "^") ||
        (PartOfOperation == "*") ||
        (PartOfOperation == "/") ||
        (PartOfOperation == "+") ||
        (PartOfOperation == "-")
        ){
            std::string Num2 = Stack.back();
            Stack.pop_back();
            std::string Num1 = Stack.back();
            Stack.pop_back();
            Stack.emplace_back(EvalNums(Num1, Num2, PartOfOperation));
        } else{
            Stack.emplace_back(PartOfOperation);
        }
    }
    if(Stack.size() > 1){
        throw error::RendorException("WTH Error; Size of stack in arithmethic operation is larger then expected. Please report this as an issue on the Rendor Github.");
    } 
    std::string& FinalOutput = Stack.back();
    if(FinalOutput.find('.') != std::string::npos){
        FinalOutput = FinalOutput.substr(0, FinalOutput.find_last_not_of('0')+1);
        if(FinalOutput.find('.') == FinalOutput.size()-1){
            FinalOutput = FinalOutput.substr(0, FinalOutput.size()-1);
        }
    }
    return FinalOutput;
}
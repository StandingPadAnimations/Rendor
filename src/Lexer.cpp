#include "lexer.hpp"
using namespace Lex;

void AddToTokensOrMainVector(Token TokenItem, std::string Value, const std::function<void(Token, std::string)>& func){
    func(TokenItem, Value);
}

std::vector<std::pair<Token, std::string>> Lexer::Tokenize(std::string& Code){
    std::stringstream ss(Code);
    std::string LineofCode;
    std::vector<std::pair<Token, std::string>> Tokens;
    std::vector<std::pair<Token, std::string>> MainFunction;
    bool IsMainFunction = false;

    // So I don't have to copy past this over and over again 
    auto AddTokenLambda = [&IsMainFunction, &Tokens, &MainFunction](Token TokenItem, std::string Value){
        switch(IsMainFunction){
            case true:
                MainFunction.emplace_back(TokenItem, Value);
                break;
            case false:
                Tokens.emplace_back(TokenItem, Value);
                break;
        }
    };
    
    unsigned int LineNumber = 0;

    while(std::getline(ss, LineofCode)){
        ++LineNumber;
        // Check for beginning of string
        std::vector<ID> TempIDList {ID::None};
        SpecificID SpecificTempID = SpecificID::None; 
        std::string TempString = "";
        ID TempID = TempIDList.back();

        for(size_t i = 0; i < LineofCode.size(); ++i){
            TempID = TempIDList.back();

            // Processing for strings
            if(TempID == ID::Char){
                switch(LineofCode[i]){
                    case '\"':
                    case '\'':
                    case '`':
                        AddToTokensOrMainVector(Token::String, TempString, AddTokenLambda);
                        TempIDList.pop_back();
                        TempString = "";
                        break;

                    default:
                        TempString.push_back(LineofCode[i]);
                }
            }
            
            // Checks for beginning and end of a string
            else if(
            ((LineofCode[i] == '\"') || 
            (LineofCode[i] == '\'') || 
            (LineofCode[i] == '`')) && 
            ((TempID == ID::None) || 
            (TempID == ID::KeywordArgs))
            ){
                TempIDList.emplace_back(ID::Char);
                TempString = "";

                // defines what should end string
                switch(LineofCode[i]){
                    case '\'': 
                        SpecificTempID = SpecificID::CharSingle;
                        break;

                    case '\"':
                        SpecificTempID = SpecificID::CharDouble;
                        break;

                    case '`':
                        SpecificTempID = SpecificID::CharTilda;
                        break;
                }
            }

            else if((LineofCode[i] == ' ') && (TempID != ID::Char)){

            }

            // Comments and division
            else if((LineofCode[i] == '/') && (TempID != ID::Char)){
                switch(TempID){
                    case ID::Number:
                        TempString.push_back(LineofCode[i]);
                        break;

                    case ID::Comment:
                        goto BreakForComment;

                    default:
                        TempIDList.emplace_back(ID::Comment);
                        TempString = "";
                        break;
                }
                // to avoid Rendor from accidentally breaking for comment for division 
                continue;
                BreakForComment:
                    break;
            }

            // Checks for operators
            else if((std::find(std::begin(Operators), std::end(Operators), TempString) != std::end(Operators)) && (TempID == ID::KeywordArgs)){
                AddToTokensOrMainVector(Token::Operator, TempString, AddTokenLambda);
                TempString = "";

                // to account for chars not being added when a operator is found
                TempString.push_back(LineofCode[i]);
            }

            //Checks for parens
            else if(LineofCode[i] == '(' && (TempID != ID::Char)){
                // Sometimes the ( is part of a keyword
                if((std::find(std::begin(Keywords), std::end(Keywords), TempString) != std::end(Keywords)) && (TempID == ID::None)){
                    AddToTokensOrMainVector(Token::Keyword, TempString, AddTokenLambda);
                    TempIDList.emplace_back(ID::KeywordArgs);
                    TempID = TempIDList.back();
                    TempString = "";
                }
                switch(TempID){
                    case ID::KeywordArgs:
                    case ID::None:
                        AddToTokensOrMainVector(Token::Paren, "PAREN", AddTokenLambda);
                        break;

                    default:
                        TempString.push_back(LineofCode[i]);
                        SpecificTempID = SpecificID::Arithmic;
                        break;
                }
            }

            else if(LineofCode[i] == ')' && (TempID != ID::Char)){
                switch(TempID){
                    case ID::KeywordArgs:
                        AddToTokensOrMainVector(Token::ArgumentObject, TempString, AddTokenLambda);
                        AddToTokensOrMainVector(Token::Paren, "PAREN", AddTokenLambda);
                        break;

                    default:
                        if(std::find(std::begin(TempIDList), std::end(TempIDList), ID::KeywordArgs) != std::end(TempIDList)){ // This is where TempIDList is useful
                            AddToTokensOrMainVector(Token::ArgumentObject, TempString, AddTokenLambda);
                            AddToTokensOrMainVector(Token::Paren, "PAREN", AddTokenLambda);
                            TempIDList.pop_back();
                            break;
                        }
                        TempString.push_back(LineofCode[i]);
                        SpecificTempID = SpecificID::Arithmic;
                        break;
                }
            }

            else if(LineofCode[i] == ',' && (TempID != ID::Char)){
                switch(TempID){
                    case ID::KeywordArgs:
                        AddToTokensOrMainVector(Token::ArgumentObject, TempString, AddTokenLambda);
                        AddToTokensOrMainVector(Token::Paren, "PAREN", AddTokenLambda);
                        break;

                    default:
                        TempString.push_back(LineofCode[i]);
                        SpecificTempID = SpecificID::Arithmic;
                        break;
                }
            }

            else if(((LineofCode[i] == '{') || (LineofCode[i] == '}')) && (TempID != ID::Char)){
                switch(LineofCode[i]){
                    case '{':
                        switch(TempID){
                            // Whenever { is used outside of a string, TempID will always be ID::KeywordArgs
                            case ID::KeywordArgs:
                                AddToTokensOrMainVector(Token::Bracket, "BRACK", AddTokenLambda);
                                TempIDList.pop_back();
                                TempString = "";
                                break;
                            
                            default:
                                throw error::RendorException("Random { found on line " + std::to_string(LineNumber));
                        }
                        break;

                    case '}':
                        AddToTokensOrMainVector(Token::Bracket, "BRACK", AddTokenLambda);
                        break;
                }
            }

            // Preprocessor definitions 
            else if((LineofCode[i] == '#') && (TempID != ID::Char)){
                std::string PreProcessFunctionName = LineofCode.substr(0, LineofCode.find_first_of(' '));
                if(PreProcessFunctionName == "#rdef"){
                    IsMainFunction = true;
                    AddToTokensOrMainVector(Token::EntryFunction, "ENTRY", AddTokenLambda);
                    TempIDList.emplace_back(ID::KeywordArgs);
                }
                else if(PreProcessFunctionName == "#rdef_end"){
                    AddToTokensOrMainVector(Token::EndOfProgram, "END", AddTokenLambda);
                    IsMainFunction = false;
                }
            }
            
            else if(LineofCode[i] == '='){
                switch(TempID){
                    case ID::None:
                        if(TempString.size() == 0){
                            throw error::RendorException("Random = found on line " + std::to_string(LineNumber));
                        }
                        AddToTokensOrMainVector(Token::Variable, TempString, AddTokenLambda);
                        TempString = "";
                        break;
                    
                    case ID::KeywordArgs:
                        if(
                        (TempString != "!") || 
                        (TempString != ">") || 
                        (TempString != "<") || 
                        (TempString != "=")
                        ){
                            AddToTokensOrMainVector(Token::ComparisonObject, TempString, AddTokenLambda); // Add what is being compared
                            TempString = "";
                        }
                        TempString.push_back(LineofCode[i]);
                        break;

                    default:
                        throw error::RendorException("Random = found on line " + std::to_string(LineNumber));
                }
            }
            
            else if(
            (LineofCode[i] == '!') || 
            (LineofCode[i] == '>') || 
            (LineofCode[i] == '<')
            ){
                switch(TempID){
                    case ID::KeywordArgs:
                        AddToTokensOrMainVector(Token::ComparisonObject, TempString, AddTokenLambda); 
                        TempString = "";
                        TempString.push_back(LineofCode[i]);
                        break;

                    default:
                        throw error::RendorException("Random operator found on line " + std::to_string(LineNumber));
                }
            }

            else if(
                ((std::isdigit(LineofCode[i])) || 
                (LineofCode[i] == '.')) && 
                (TempID != ID::Char)
                ){
                switch(TempID){
                    case ID::Number:
                        TempString.push_back(LineofCode[i]);
                        if(LineofCode[i] == '.'){
                            switch(SpecificTempID){
                                case SpecificID::Int:
                                    SpecificTempID = SpecificID::Float;
                                    break;
                                
                                default:
                                    break;
                            }
                        }
                        switch(SpecificTempID){
                            case SpecificID::IncrementDecrementCheck:
                                SpecificTempID = SpecificID::Arithmic;

                            default:
                                break;
                        }
                        break;
                    
                    default:
                        switch(SpecificTempID){
                            case SpecificID::Int:
                                break;
                            
                            default:
                                SpecificTempID = SpecificID::Int; // floats can't be defined without an int first 
                        }
                        TempIDList.emplace_back(ID::Number);
                        TempID = TempIDList.back();
                        TempString.push_back(LineofCode[i]);
                        break;
                }
            }

            else if(
            (LineofCode[i] == '^') ||
            (LineofCode[i] == '*') || 
            (LineofCode[i] == '+') || 
            (LineofCode[i] == '-') 
            ){
                switch(LineofCode[i]){
                    case '^':
                        TempString.push_back(LineofCode[i]);
                        break;

                    case '*':
                        TempString.push_back(LineofCode[i]);
                        break;

                    case '+':
                        switch(SpecificTempID){
                            case SpecificID::IncrementDecrementCheck:
                                TempString.push_back(LineofCode[i]);
                                SpecificTempID = SpecificID::Increment;
                                break;

                            default:
                                SpecificTempID = SpecificID::IncrementDecrementCheck;
                                TempString.push_back(LineofCode[i]);
                                break;
                        }
                        break;

                    case '-':
                        switch(SpecificTempID){
                            case SpecificID::IncrementDecrementCheck:
                                TempString.push_back(LineofCode[i]);
                                SpecificTempID = SpecificID::Decrement;
                                break;

                            default:
                                SpecificTempID = SpecificID::IncrementDecrementCheck;
                                TempString.push_back(LineofCode[i]);
                                break;
                        }
                        break;
                }
                switch(SpecificTempID){
                    case SpecificID::Increment:
                    case SpecificID::Decrement:
                    case SpecificID::IncrementDecrementCheck:
                        break;
                    
                    case SpecificID::Arithmic:
                        break;

                    default:
                        SpecificTempID = SpecificID::Arithmic;
                }
                switch(TempID){
                    case ID::Number:
                        break;
                    
                    default:
                        TempIDList.emplace_back(ID::Number);
                }
            }

            // Push chars in a temporary string
            else{
                TempString.push_back(LineofCode[i]);
                // std::cout << static_cast<std::underlying_type<ID>::type>(TempID) << std::endl;
            }
        }
        if(TempID == ID::Number){
            switch(SpecificTempID){
                case SpecificID::Int:
                    AddToTokensOrMainVector(Token::Int, TempString, AddTokenLambda);
                    break;

                case SpecificID::Float:
                    AddToTokensOrMainVector(Token::Float, TempString, AddTokenLambda);
                    break;

                case SpecificID::Arithmic:
                    AddToTokensOrMainVector(Token::Arithmic, TempString, AddTokenLambda);
                    break;

                case SpecificID::Increment:
                    AddToTokensOrMainVector(Token::Increment, TempString, AddTokenLambda);
                    break;

                case SpecificID::Decrement:
                    AddToTokensOrMainVector(Token::Decrement, TempString, AddTokenLambda);
                    break;
                
                default:
                    throw error::RendorException("Massive screw up on line " + std::to_string(LineNumber));
            }
        }
    }
    for(auto const& pair : MainFunction){
        Tokens.emplace_back(pair);
    }
    return Tokens;
}
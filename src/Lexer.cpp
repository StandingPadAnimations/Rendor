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

        for(size_t i = 0; i < LineofCode.size(); ++i){
            ID TempID = TempIDList.back();
            // Checks for beginning and end of a string
            if((LineofCode[i] == '\"') || (LineofCode[i] == '\'') || (LineofCode[i] == '`')){
                // beggining of a string
                if((TempID == ID::None) || (TempID == ID::KeywordArgs)){
                    // std::cout << "StringStart " << TempString << std::endl;
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

                // end of a string
                else if(TempID == ID::Char){
                    // std::cout << TempString << std::endl;

                    if(((LineofCode[i] == '\'') && (SpecificTempID == SpecificID::CharSingle)) || 
                    ((LineofCode[i] == '\"') && (SpecificTempID == SpecificID::CharDouble)) || 
                    ((LineofCode[i] == '`') && (SpecificTempID == SpecificID::CharTilda))){
                        AddToTokensOrMainVector(Token::String, TempString, AddTokenLambda);
                        TempIDList.pop_back();
                        TempString = "";
                    }
                    else{
                        TempString.push_back(LineofCode[i]);
                    }
                }

                else{
                    // std::cout << "StringEOL Error " << TempString  << " " << static_cast<std::underlying_type<ID>::type>(TempID) << std::endl;
                    throw error::RendorException("Invalid Syntax: EOL while scanning string on line " + std::to_string(LineNumber));
                }
            }

            else if((LineofCode[i] == ' ') && (TempID != ID::Char)){

            }

            // Comments
            else if((LineofCode[i] == '/') && (TempID != ID::Char)){
                if(TempID != ID::Comment){
                    // std::cout << "Encounted Comment" << std::endl;
                    TempIDList.emplace_back(ID::Comment);
                    TempString = "";
                }
                
                else if(TempID == ID::Comment){
                    // std::cout << "Comment " << TempString << std::endl; 
                    break;
                }
            }

            // Checks for operators
            else if((std::find(std::begin(Operators), std::end(Operators), TempString) != std::end(Operators)) && (TempID == ID::KeywordArgs)){
                AddToTokensOrMainVector(Token::Operator, TempString, AddTokenLambda);
                TempString = "";

                // to account for chars not being added when a operator is found
                TempString.push_back(LineofCode[i]);
            }

            //Checks for parens
            else if(LineofCode[i] == '('){
                // Sometimes the ( is part of a keyword
                if((std::find(std::begin(Keywords), std::end(Keywords), TempString) != std::end(Keywords)) && (TempID == ID::None)){
                    AddToTokensOrMainVector(Token::Keyword, TempString, AddTokenLambda);
                    TempIDList.emplace_back(ID::KeywordArgs);
                    TempString = "";
                }
                AddToTokensOrMainVector(Token::Paren, "PAREN", AddTokenLambda);
            }

            else if(LineofCode[i] == ')'){
                AddToTokensOrMainVector(Token::Paren, "PAREN", AddTokenLambda);
            }

            else if((LineofCode[i] == '{') || (LineofCode[i] == '}')){
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
                        AddToTokensOrMainVector(Token::Variable, TempString, AddTokenLambda);
                        TempString = "";
                        break;
                    
                    case ID::KeywordArgs:
                        if(SpecificTempID != SpecificID::EqualOperator){
                            SpecificTempID = SpecificID::EqualOperator;
                            break;
                        }
                        AddToTokensOrMainVector(Token::ComparisonObject, TempString, AddTokenLambda);
                        AddToTokensOrMainVector(Token::Operator, "EQUAL", AddTokenLambda);
                        TempString = "";
                        break;

                    default:
                        throw error::RendorException("Random = found on line " + std::to_string(LineNumber));
                }
            }

            else if(LineofCode[i] == '^' ||
            LineofCode[i] == '*' || LineofCode[i] == '/' ||
            LineofCode[i] == '+' || LineofCode[i] == '-'){
                switch(LineofCode[i]){
                    case '^':
                        AddToTokensOrMainVector(Token::Arithmic, "^", AddTokenLambda);
                        break;

                    case '*':
                        AddToTokensOrMainVector(Token::Arithmic, "*", AddTokenLambda);
                        break;

                    case '/':
                        AddToTokensOrMainVector(Token::Arithmic, "/", AddTokenLambda);
                        break;

                    case '+':
                        switch(SpecificTempID){
                            case SpecificID::IncrementDecrementCheck:
                                Tokens.pop_back();
                                AddToTokensOrMainVector(Token::Increment, "++", AddTokenLambda);
                                break;

                            default:
                                AddToTokensOrMainVector(Token::Arithmic, "+", AddTokenLambda);
                                SpecificTempID = SpecificID::IncrementDecrementCheck;
                                break;
                        }
                        break;

                    case '-':
                        switch(SpecificTempID){
                            case SpecificID::IncrementDecrementCheck:
                                Tokens.pop_back();
                                AddToTokensOrMainVector(Token::Decrement, "--", AddTokenLambda);
                                break;

                            default:
                                AddToTokensOrMainVector(Token::Arithmic, "-", AddTokenLambda);
                                SpecificTempID = SpecificID::IncrementDecrementCheck;
                                break;
                        }
                        break;
                }
            }

            // Push chars in a temporary string
            else{
                TempString.push_back(LineofCode[i]);
                // std::cout << static_cast<std::underlying_type<ID>::type>(TempID) << std::endl;
            }
        }
    }
    for(auto const& pair : MainFunction){
        Tokens.emplace_back(pair);
    }
    return Tokens;
}
#include "lexer.hpp"
using namespace Lex;


std::vector<std::pair<Token, std::string>> Lexer::Tokenize(std::string& Code){
    std::stringstream ss(Code);
    std::string LineofCode;
    std::vector<std::pair<Token, std::string>> Tokens;
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
                        Tokens.emplace_back(Token::String, TempString);
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
                Tokens.emplace_back(Token::Operator, TempString);
                TempString = "";

                // to account for chars not being added when a operator is found
                TempString.push_back(LineofCode[i]);
            }

            //Checks for parens
            else if(LineofCode[i] == '('){
                // Sometimes the ( is part of a keyword
                if((std::find(std::begin(Keywords), std::end(Keywords), TempString) != std::end(Keywords)) && (TempID == ID::None)){
                    Tokens.emplace_back(Token::Keyword, TempString);
                    TempIDList.emplace_back(ID::KeywordArgs);
                    TempString = "";
                }
                Tokens.emplace_back(Token::Paren, "PAREN");
            }

            else if(LineofCode[i] == ')'){
                Tokens.emplace_back(Token::Paren, "PAREN");
            }

            else if((LineofCode[i] == '{') || (LineofCode[i] == '}')){
                switch(LineofCode[i]){
                    case '{':
                        switch(TempID){
                            // Whenever { is used outside of a string, TempID will always be ID::KeywordArgs
                            case ID::KeywordArgs:
                                Tokens.emplace_back(Token::Bracket, "BRACK");
                                TempIDList.pop_back();
                                TempString = "";
                                break;
                            
                            default:
                                throw error::RendorException("Random { found on line " + std::to_string(LineNumber));
                        }
                        break;

                    case '}':
                        Tokens.emplace_back(Token::Bracket, "BRACK");
                        break;
                }
            }

            // Preprocessor definitions are planned for a future update 
            else if((LineofCode[i] == '#') && (TempID != ID::Char)){
                throw error::RendorException("Preprocessor definitions are currently not supported in Rendor 1.0");
            }

            else if(LineofCode[i] == '='){
                switch(TempID){
                    case ID::None:
                        Tokens.emplace_back(Token::Variable, TempString);
                        TempString = "";
                        break;
                    
                    case ID::KeywordArgs:
                        if(SpecificTempID != SpecificID::EqualOperator){
                            SpecificTempID = SpecificID::EqualOperator;
                            break;
                        }
                        Tokens.emplace_back(Token::ComparisonObject, TempString);
                        Tokens.emplace_back(Token::Operator, "EQUAL");
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
                        Tokens.emplace_back(Token::Arithmic, "^");
                        break;

                    case '*':
                        Tokens.emplace_back(Token::Arithmic, "*");
                        break;

                    case '/':
                        Tokens.emplace_back(Token::Arithmic, "/");
                        break;

                    case '+':
                        switch(SpecificTempID){
                            case SpecificID::IncrementDecrementCheck:
                                Tokens.pop_back();
                                Tokens.emplace_back(Token::Increment, "++");
                                break;

                            default:
                                Tokens.emplace_back(Token::Arithmic, "+");
                                SpecificTempID = SpecificID::IncrementDecrementCheck;
                                break;
                        }
                        break;

                    case '-':
                        switch(SpecificTempID){
                            case SpecificID::IncrementDecrementCheck:
                                Tokens.pop_back();
                                Tokens.emplace_back(Token::Decrement, "--");
                                break;

                            default:
                                Tokens.emplace_back(Token::Arithmic, "-");
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
    return Tokens;
}
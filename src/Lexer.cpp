#include "lexer.hpp"
using namespace Lex;


std::vector<std::pair<Token, std::string_view>> Lexer::Tokenize(std::string& Code){
    std::stringstream ss(Code);
    std::string LineofCode;
    std::vector<std::pair<Token, std::string_view>> Tokens;
    unsigned int LineNumber = 0;

    while(std::getline(ss, LineofCode)){
        ++LineNumber;
        // Check for beginning of string
        ID TempID = ID::None;
        std::string TempString = "";

        for(size_t i = 0; i < LineofCode.size(); ++i){
            // Checks for beginning and end of a string
            if((LineofCode[i] == '\"') || (LineofCode[i] == '\'')){
                // beggining of a string
                if((TempID == ID::None) || (TempID == ID::ParenArgs)){
                    TempID = ID::Char;
                    TempString = "";
                }

                // end of a string
                else if(TempID == ID::Char){
                    std::cout << TempString << std::endl;
                    TempID = ID::StringEnd;
                    Tokens.emplace_back(Token::String, TempString);
                    TempString = "";
                }

                else{
                    std::cout << TempString << std::endl;
                    throw error::RendorException("Invalid Syntax: EOL while scanning string on line " + std::to_string(LineNumber));
                }
            }

            else if((LineofCode[i] == ' ') && (TempID != ID::Char)){
                
            }

            // Comments
            else if((LineofCode[i] == '/') && (TempID != ID::Char)){
                if(TempID != ID::Comment){
                    TempID = ID::Comment;
                    TempString = "";
                }
                
                else if(TempID == ID::Comment){
                    break;
                }
            }

            // Checks for keywords
            else if((std::find(std::begin(Keywords), std::end(Keywords), TempString) != std::end(Keywords)) && (TempID == ID::None)){
                std::cout << TempString << std::endl;
                Tokens.emplace_back(Token::Keyword, TempString);
                TempID = ID::KeywordArgs;
                TempString = "";
            }

            //Checks for parens
            else if((LineofCode[i] == '(') && (TempID == ID::KeywordArgs)){
                Tokens.emplace_back(Token::Paren, TempString);
                TempID = ID::ParenArgs;
                TempString = "";
            }

            else if((LineofCode[i] == ')') && (TempID == ID::ParenArgs)){
                if(TempID == ID::ParenArgs){
                    Tokens.emplace_back(Token::Paren, TempString);
                    TempID = ID::None;
                    TempString = "";
                }

                // in case a random ) is found
                else{
                    throw error::RendorException("Invalid Syntax: Random ) found on line " + LineNumber);
                }
            }

            // Whenever { is used outside of a string, TempID will always be ID::None
            else if((LineofCode[i] == '{') && (TempID == ID::None)){
                Tokens.emplace_back(Token::Bracket, TempString);
                TempID = ID::None;
                TempString = "";
            }

            else if((LineofCode[i] == '|') && (TempID == ID::ParenArgs)){
                Tokens.emplace_back(Token::Operator, TempString);
                TempString = "";
            }

            // Push chars in a temporary string
            else{
                TempString.push_back(LineofCode[i]);
            }
        }
    }
    return Tokens;
}
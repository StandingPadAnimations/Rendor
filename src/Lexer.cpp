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
        std::vector<ID> TempID {ID::None};
        std::string TempString = "";

        for(size_t i = 0; i < LineofCode.size(); ++i){
            // Checks for beginning and end of a string
            if((LineofCode[i] == '\"') || (LineofCode[i] == '\'')){
                // beggining of a string
                if((TempID.back() == ID::None) || (TempID.back() == ID::ParenArgs) || (TempID.back() == ID::KeywordArgs)){
                    // std::cout << "StringStart " << TempString << std::endl;
                    TempID.emplace_back(ID::Char);
                    TempString = "";
                }

                // end of a string
                else if(TempID.back() == ID::Char){
                    // std::cout << TempString << std::endl;
                    Tokens.emplace_back(Token::String, TempString);
                    TempID.pop_back();
                    TempString = "";
                }

                else{
                    // std::cout << "StringEOL Error " << TempString  << " " << static_cast<std::underlying_type<ID>::type>(TempID.back()) << std::endl;
                    throw error::RendorException("Invalid Syntax: EOL while scanning string on line " + std::to_string(LineNumber));
                }
            }

            else if((LineofCode[i] == ' ') && (TempID.back() != ID::Char)){

            }

            // Comments
            else if((LineofCode[i] == '/') && (TempID.back() != ID::Char)){
                if(TempID.back() != ID::Comment){
                    // std::cout << "Encounted Comment" << std::endl;
                    TempID.emplace_back(ID::Comment);
                    TempString = "";
                }
                
                else if(TempID.back() == ID::Comment){
                    // std::cout << "Comment " << TempString << std::endl; 
                    break;
                }
            }

            // Checks for keywords
            else if((std::find(std::begin(Keywords), std::end(Keywords), TempString) != std::end(Keywords)) && (TempID.front() == ID::None)){
                // std::cout << "Keyword " << TempString << std::endl;
                Tokens.emplace_back(Token::Keyword, TempString);
                TempID.emplace_back(ID::KeywordArgs);
                TempString = "";
            }

            //Checks for parens
            else if(((LineofCode[i] == '(') || (LineofCode[i] == ')')) && (TempID.back() == ID::KeywordArgs)){
                Tokens.emplace_back(Token::Paren, "PAREN");
                TempString = "";
            }

            // Whenever { is used outside of a string, TempID will always be ID::None
            else if((LineofCode[i] == '{') && (TempID.back() == ID::None)){
                Tokens.emplace_back(Token::Bracket, TempString);
                TempID.pop_back();
                TempString = "";
            }

            else if((LineofCode[i] == '#') && (TempID.back() != ID::Char)){
                throw error::RendorException("Preprocessor definitions are currently not supported in Rendor 1.0");
            }

            else if((LineofCode[i] == '|') && (TempID.back() == ID::KeywordArgs)){
                std::cout << "Reached Operator " << TempString << std::endl;
                Tokens.emplace_back(Token::Operator, "OR");
                TempString = "";
            }

            else if((LineofCode[i] == '&') && (TempID.back() == ID::KeywordArgs)){
                std::cout << "Reached Operator " << TempString << std::endl;
                Tokens.emplace_back(Token::Operator, "AND");
                TempString = "";
            }

            // Push chars in a temporary string
            else{
                TempString.push_back(LineofCode[i]);
                // std::cout << static_cast<std::underlying_type<ID>::type>(TempID.back()) << std::endl;
            }
        }
    }
    return Tokens;
}
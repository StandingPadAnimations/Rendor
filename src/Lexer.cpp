#include "lexer.hpp"
using namespace Lex;


std::vector<std::pair<Token, std::string_view>> Lexer::Tokenize(std::string& Code){
    std::stringstream ss(Code);
    std::string LineofCode;
    std::vector<std::pair<Token, std::string_view>> Tokens;

    while(std::getline(ss, LineofCode)){
        // Check for beginning of string
        ID TempID = ID::None;
        std::string TempString = "";

        for(size_t i = 0; i < LineofCode.size(); ++i){
            // Checks for beginning of string
            if(((LineofCode[i] == '\"') || (LineofCode[i] == '\'')) && (TempID == ID::None)){
                TempID = ID::Char;
                TempString = "";
            }

            // Check for end of string
            else if(((LineofCode[i] == '\"') || (LineofCode[i] == '\'')) && (TempID == ID::Char)){
                std::cout << TempString << std::endl;
                TempID = ID::StringEnd;
                Tokens.emplace_back(Token::String, TempString);
                TempString = "";
            }

            // Start of a comment
            else if(((LineofCode[i] == '/') && (TempID == ID::None))){
                TempID = ID::Comment;
                TempString = "";
            }

            // end of comment
            else if(((LineofCode[i] == '/') && (TempID == ID::Comment))){
                break;
            }

            // Checks for keywords
            else if((std::find(std::begin(Keywords), std::end(Keywords), TempString) != std::end(Keywords)) && (TempID != ID::Char)){
                Tokens.emplace_back(Token::Keyword, TempString);
                TempID = ID::KeywordArgs;
            }

            //Checks for parens
            else if((LineofCode[i] == '(') && (TempID == ID::KeywordArgs)){
                Tokens.emplace_back(Token::Paren, TempString);
                TempID = ID::ParenArgs;
                TempString = "";
            }

            // Push chars in a temporary string
            else{
                TempString.push_back(LineofCode[i]);
            }
        }
    }
}
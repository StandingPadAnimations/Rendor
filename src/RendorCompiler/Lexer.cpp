#include "RendorCompiler/Lexer.hpp"
using namespace Lex;

std::vector<std::pair<Token, std::string>> Lexer::Tokenize (const std::string& Code, std::string_view ParentPath){ // ? Perhaps string_view be used over const std::string&
    std::vector<std::pair<Token, std::string>> Tokens;
    std::string Buffer = "";

    for (size_t Char; Char < Code.size(); ++Char)
    { 
        // When we come across a space, symbol, or newline
        if 
        ((Code[Char] == '\n') ||
        (Code[Char] == ' ') ||
        (Code[Char] == '(') ||
        (Code[Char] == ')'))
        {
            if (std::find(Keywords.begin(), Keywords.end(), Buffer) != Keywords.end()) // if it is a keyword
            {
                Tokens.emplace_back(Token::KEYWORD, Buffer);
                Buffer = "";
            }
            else 
            {
                Tokens.emplace_back(Token::IDENTIFIER, Buffer);
                Buffer = "";
            }
        }

        else 
        {
            Buffer.push_back(Code[Char]);
        }
    }
    return Tokens; 
}
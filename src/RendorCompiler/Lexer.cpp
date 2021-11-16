#include "RendorCompiler/Lexer.hpp"
using namespace Lex;

std::vector<std::pair<Token, std::string>> Lexer::Tokenize (const std::string& Code, std::string_view ParentPath){
    std::vector<std::pair<Token, std::string>> Tokens;
    std::string_view Buffer = "";
    size_t StartIndex = 0;
    size_t EndIndex = 0;

    for (size_t Char; Char < Code.size(); ++Char)
    { 
        // When we come across a space, symbol, or newline
        if 
        ((Code[Char] == '\n') ||
        (Code[Char] == ' ') ||
        (Code[Char] == '(') ||
        (Code[Char] == ')'))
        {
            Buffer.substr(StartIndex, EndIndex);

            if (Buffer.find_first_not_of(' ') == std::string::npos)
            {
                // Does nothing except invalidate the else if and else statements so the compiler goes to the switch statement
            }

            // Keywords and functions
            else if (std::find(Keywords.begin(), Keywords.end(), Buffer) != Keywords.end()) // if it is a keyword
            {
                Tokens.emplace_back(Token::KEYWORD, std::string{Buffer});
            }
            
            else if (std::find(Functions.begin(), Functions.end(), Buffer) != Functions.end()) // if it is a built in function
            {
                Tokens.emplace_back(Token::BUILT_IN_FUNCTION, std::string{Buffer});
            }

            // Identifiers
            else 
            {
                Tokens.emplace_back(Token::IDENTIFIER, std::string{Buffer});
            }

            // Tokenizing the characthers we stop on 
            switch (Code[Char])
            {
                case ' ': 
                    break; // Point of lexer is to remove whitespace so parser has an easier time

                case '(':
                    Tokens.emplace_back(Token::LPAREN, "");
                    break;

                case ')':
                    Tokens.emplace_back(Token::RPAREN, "");
                    break;

                case '\n':
                    Tokens.emplace_back(Token::NEWLINE, "");
            }

            StartIndex = EndIndex + 1;
        }

        else 
        {
            ++EndIndex;
        }
    }
    return Tokens; 
}
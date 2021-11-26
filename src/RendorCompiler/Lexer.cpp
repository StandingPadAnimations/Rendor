#include "RendorCompiler/Lexer.hpp"
using namespace Lex;

std::vector<std::pair<Token, std::string>> Lexer::Tokenize (const std::string& Code, std::string_view ParentPath){
    std::vector<std::pair<Token, std::string>> Tokens;
    BufferID LexerBufferID = BufferID::None;
    size_t StartIndex = 0;
    size_t EndIndex = 0;

    for (size_t Char = 0; Char < Code.size(); ++Char)
    { 
        // When we come across a space, symbol, or newline
        if  
        (((Code[Char] == ' ') ||
        (Code[Char] == ';')   ||
        (Code[Char] == ',')   ||
        (Code[Char] == '(')   ||
        (Code[Char] == ')')   ||
        (Code[Char] == '{')   ||
        (Code[Char] == '}')   ||
        (Code[Char] == '='))  &&
        ((LexerBufferID == BufferID::None) ||
        (LexerBufferID == BufferID::StringEnd) ||
        LexerBufferID == BufferID::Comment))
        {
            std::string_view Buffer(Code.begin() + StartIndex, Code.begin() + (EndIndex));

            if (Buffer.find_first_not_of(" ;,(){}=") == std::string::npos)
            {
                // Does nothing except invalidate the else if and else statements so the compiler goes to the switch statement
            }

            else if (LexerBufferID == BufferID::StringEnd)
            {
                LexerBufferID = BufferID::None; // resets BufferID when needed
            }

            else if 
            ((LexerBufferID == BufferID::Comment) &&
            (Code[Char] == ';'))
            {
                LexerBufferID = BufferID::None; // resets BufferID when needed
            }

            else if (LexerBufferID == BufferID::None) // Everything that requires the BufferID to be None
            {
                // Keywords 
                if (std::find(Keywords.begin(), Keywords.end(), Buffer) != Keywords.end()) // if it is a keyword
                {
                    Tokens.emplace_back(Token::KEYWORD, std::string{Buffer});
                }
                
                // Functions
                else if (std::find(Functions.begin(), Functions.end(), Buffer) != Functions.end()) // if it is a built in function
                {
                    Tokens.emplace_back(Token::BUILT_IN_FUNCTION, std::string{Buffer});
                }

                // Floats 
                else if 
                ((Buffer.find_last_not_of("1234567890.") == std::string::npos) &&
                (Buffer.find_first_of(".") != std::string::npos))
                {
                    Tokens.emplace_back(Token::FLOAT, std::string{Buffer});
                }

                // Ints
                else if (Buffer.find_last_not_of("1234567890") == std::string::npos)
                {
                    Tokens.emplace_back(Token::INT, std::string{Buffer});
                }

                // Booleans 
                else if 
                ((Buffer == "true") ||
                (Buffer == "false"))
                {
                    Tokens.emplace_back(Token::BOOL, std::string{Buffer});
                }

                // Identifiers
                else 
                {
                    Tokens.emplace_back(Token::IDENTIFIER, std::string{Buffer});
                }
            }

            StartIndex = Code.find_first_not_of(' ', EndIndex);

            // Tokenizing the characthers we stop on 
            switch (Code[Char])
            {
                case ' ': 
                    break; // Point of lexer is to remove whitespace so parser has an easier time

                case ';':
                    Tokens.emplace_back(Token::NEWLINE, ";");
                    ++StartIndex;
                    break;

                case ',':
                    Tokens.emplace_back(Token::COMMA, ",");
                    ++StartIndex;
                    break;

                case '(':
                    Tokens.emplace_back(Token::LPAREN, "(");
                    ++StartIndex;
                    break;

                case ')':
                    Tokens.emplace_back(Token::RPAREN, ")");
                    break;

                case '{':
                    Tokens.emplace_back(Token::LBRACE, "{");
                    break;

                case '}':
                    Tokens.emplace_back(Token::RBRACE, "}");
                    break;

                case '=':
                    Tokens.emplace_back(Token::EQUAL, "=");
                    break;

            }
            
        }

        else if
        ((Code[Char] == '/') &&
        (Code[Char + 1] == '/'))
        {
            LexerBufferID = BufferID::Comment;
        }

        // quotes 
        else if 
        ((Code[Char] == '\'') ||
        (Code[Char] == '"')  ||
        (Code[Char] == '`'))
        {
            if (LexerBufferID == BufferID::None) // Start of a string 
            {
                switch (Code[Char])
                {
                    case '\'':
                        LexerBufferID = BufferID::CharSingle;
                        break;
                    
                    case '"':
                        LexerBufferID = BufferID::CharDouble;
                        break;
                        
                    case '`':
                        LexerBufferID = BufferID::CharTilda;
                        break;
                }
                StartIndex = Code.find_first_not_of(' ', EndIndex);
            }

            else 
            {
                char CharToCheck; // For checking matching char for closing string
                switch (LexerBufferID){
                    case BufferID::CharSingle:
                        CharToCheck = '\'';
                        break;
                    
                    case BufferID::CharDouble:
                        CharToCheck = '"';
                        break;

                    case BufferID::CharTilda:
                        CharToCheck = '`';
                        break;
                    
                    default:
                        break;
                }

                if (Code[Char] == CharToCheck)
                {
                    StartIndex = Code.find_first_not_of(CharToCheck, StartIndex); // To avoid tokenizing the ", ', or ` that starts a string
                    std::string_view Buffer(Code.begin() + StartIndex, Code.begin() + (EndIndex));
                    Tokens.emplace_back(Token::STRING, Buffer);
                    LexerBufferID = BufferID::StringEnd;
                }
                
                else
                {
                    // Ignore 
                }
            }
        }
        ++EndIndex;
    }
    return Tokens; 
}
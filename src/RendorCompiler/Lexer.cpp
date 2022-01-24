#include "RendorCompiler/Lexer.hpp"
using namespace Lex;

std::vector<std::pair<Token, std::string>> Lexer::Tokenize(const std::string& Code){
    std::vector<std::pair<Token, std::string>> Tokens;
    BufferID LexerBufferID = BufferID::None;
    size_t StartIndex = 0;
    size_t EndIndex = 0;

    for (size_t Char = 0; Char < Code.size(); ++Char)
    { 
        /* -------------------------------------------------------------------------- */
        /*                            During certain states                           */
        /* -------------------------------------------------------------------------- */
        if 
        (((LexerBufferID == BufferID::CharSingle) ||
        (LexerBufferID   == BufferID::CharDouble) ||
        (LexerBufferID   == BufferID::CharTilda)) &&
        ((Code[Char]     != '\'')                 &&
        (Code[Char]      != '"')                  &&
        (Code[Char]      != '`')))
        {
            
        }

        else if 
        ((LexerBufferID == BufferID::Comment) &&
        (Code[Char] != ';'))
        {
            
        }

        /* -------------------------------------------------------------------------- */
        /*               When we come across a space, symbol, or newline              */
        /* -------------------------------------------------------------------------- */
        else if  
        ((LexerCharCheck(Code[Char]))  &&
        ((LexerBufferID == BufferID::None)      ||
        (LexerBufferID  == BufferID::StringEnd) ||
        LexerBufferID   == BufferID::Comment))
        {
            /* ----------------------- Checking if it's a comment ----------------------- */
            if
            ((Code[Char] == '/') &&
            (Code[Char + 1] == '/'))
            {
                LexerBufferID = BufferID::Comment;
                ++EndIndex;
                continue;
            }
            
            std::string_view Buffer(Code.begin() + StartIndex, Code.begin() + (EndIndex));

            if (Buffer.size() > 1)
            {
                while(LexerCharCheck(Buffer[0]))
                {
                    ++StartIndex;
                    Buffer = std::string_view(Code.begin() + StartIndex, Code.begin() + (EndIndex));
                }
            }
            
            else if (Buffer.size() == 0)
            {
                // * Because this used to cause a bug with comments
            }

            /* ------------------------- Tokenization of buffer ------------------------- */
            if 
            ((Buffer.find_first_not_of(" ;,(){}=^*/+-><") == std::string::npos) &&
            (LexerBufferID != BufferID::Comment))
            {
                std::string BufferAsString{Buffer};
                
                // Check if the buffer contains a token that can be part of a larger token
                if (BiOpTokens.contains(BufferAsString))
                {
                    // Check if it's compatible with the current characther 
                    if (BiOpTokens[BufferAsString] == Code[Char])
                    {
                        auto& [Token, value] = Tokens.back();
                        Token = Lex::Token::BIOP;
                        value += Code[Char];
                    }
                    else 
                    {
                        // Ignore as we don't care 
                    }

                    // ? Not sure if this is a good use of goto
                    goto StartIndex;
                }

                // Check if the buffer contains a token that can be part of a larger token
                else if (UnOpTokens.contains(BufferAsString))
                {
                    // Check if it's compatible with the current characther 
                    if (UnOpTokens[BufferAsString] == Code[Char])
                    {
                        auto& [Token, value] = Tokens.back();
                        Token = Lex::Token::UnOp;
                        value += Code[Char];
                    }
                    else 
                    {
                        // Ignore as we don't care 
                    }
                    
                    // ? Not sure if this is a good use of goto
                    goto StartIndex;
                }
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

                else if (std::find(Operators.begin(), Operators.end(), Buffer) != Operators.end()) // if it is a built in function
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

            // Tokenizing the characthers we stop on 
            switch (Code[Char])
            {
                case ' ': 
                {
                    break; // Point of lexer is to remove whitespace so parser has an easier time
                }

                case ';':
                {    
                    Tokens.emplace_back(Token::NEWLINE, ";");
                    ++StartIndex;
                    break;
                }

                case ',':
                {    
                    Tokens.emplace_back(Token::COMMA, ",");
                    ++StartIndex;
                    break;
                }

                case '(':
                {    
                    Tokens.emplace_back(Token::LPAREN, "(");
                    ++StartIndex;
                    break;
                }

                case ')':
                {
                    Tokens.emplace_back(Token::RPAREN, ")");
                    break;
                }

                case '{':
                {
                    Tokens.emplace_back(Token::LBRACE, "{");
                    break;
                }

                case '}':
                {
                    Tokens.emplace_back(Token::RBRACE, "}");
                    break;
                }

                case '=':
                {
                    Tokens.emplace_back(Token::EQUAL, "=");
                    break;
                }
                
                case '^':
                    FALLTHROUGH;
                case '*':
                    FALLTHROUGH;
                case '/':
                    FALLTHROUGH;
                case '+':
                    FALLTHROUGH;
                case '-':
                    FALLTHROUGH;
                case '>':
                    FALLTHROUGH;
                case '<':
                {
                    Tokens.emplace_back(Token::BIOP, std::string{Code[Char]});
                    break;
                }
            }

            StartIndex: 
                StartIndex = Code.find_first_not_of(' ', EndIndex);
        }

        /* -------------------------------------------------------------------------- */
        /*                                   Strings                                  */
        /* -------------------------------------------------------------------------- */
        else if 
        ((Code[Char] == '\'') ||
        (Code[Char]  == '"')  ||
        (Code[Char]  == '`'))
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
                char CharToCheck = '\0'; // For checking matching char for closing string
                switch (LexerBufferID){
                    case BufferID::CharSingle:
                    {
                        CharToCheck = '\'';
                        break;
                    }
                    
                    case BufferID::CharDouble:
                    {
                        CharToCheck = '"';
                        break;
                    }

                    case BufferID::CharTilda:
                    {
                        CharToCheck = '`';
                        break;
                    }
                    
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

bool Lexer::LexerCharCheck(char Char)
{
    if (
    (Char   == ' ')   ||
    (Char   == ';')   ||
    (Char   == ',')   ||
    (Char   == '(')   ||
    (Char   == ')')   ||
    (Char   == '{')   ||
    (Char   == '}')   ||
    (Char   == '=')   ||
    (Char   == '^')   ||
    (Char   == '*')   ||
    (Char   == '/')   ||
    (Char   == '+')   ||
    (Char   == '-')   ||
    (Char   == '>')   ||
    (Char   == '<'))
    {
        return true;
    }
    return false;
}
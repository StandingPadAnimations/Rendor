#include "RendorCompiler/Lexer/Lexer.hpp"
using namespace Lex;

std::vector<std::pair<Token, std::string>> Lexer::Tokenize(const boost::interprocess::mapped_region& MappedCode){
    std::vector<std::pair<Token, std::string>> Tokens;
    BufferID LexerBufferID = BufferID::None;
    Tokens.reserve(5000);
    for (auto Line : RendorMapping::crange(MappedCode))
    {
        /* -------------------------- Current Line of code -------------------------- */
        std::string_view Code{Line.begin(), Line.end()};
        size_t StartIndex = 0;
        size_t EndIndex = 0;
        
        /* --------------------------- Add new line token --------------------------- */
        if 
        ((Code.find_first_not_of(" \r\n") == std::string_view::npos) ||
        (Code.empty()))
        {
            Tokens.emplace_back(Token::NEWLINE, "\r");
            continue;
        }

        else if 
        ((Code.find_first_not_of(" \r\n") != std::string_view::npos) ||
        (Code.find_last_not_of(" \r\n") != std::string_view::npos))
        {
            size_t Pos1 = Code.find_first_not_of(" \r\n");
            size_t Pos2 = Code.find_last_not_of(" \r\n") + (Code.size() - Code.find_last_of('\r'));
            Code = Code.substr(Pos1, Pos2);
            while (Code.back() != '\r')
            {
                switch (Code.back())
                {
                    case '\n':
                    {
                        --Pos2;
                        break;
                    }
                    default:
                    {
                        if (Pos2 > Code.size())
                        {
                            goto EXIT_LOOP;
                        }
                        ++Pos2;
                        break;
                    }
                }
                    Code = std::string_view{Line.begin(), Line.end()};
                    Code = Code.substr(Pos1, Pos2);
                EXIT_LOOP:
                    break;
            }
        }

        /* --------------------- Iterating over each characther --------------------- */
        for (size_t Char = 0; Char < Code.size(); ++Char)
        { 
            /* -------------------------------------------------------------------------- */
            /*                            During certain states                           */
            /* -------------------------------------------------------------------------- */
            if 
            (((LexerBufferID == BufferID::CharSingle) ||
            (LexerBufferID   == BufferID::CharDouble) ||
            (LexerBufferID   == BufferID::CharTilda)  ||
            (LexerBufferID   == BufferID::Comment)) &&
            ((Code[Char]     != '\'') &&
            (Code[Char]      != '"')  &&
            (Code[Char]      != '`')  &&
            (Code[Char]      != '\r')))
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
                ((Buffer.find_first_not_of(" \n\r;,(){}=^*/+-><:") == std::string::npos) &&
                (LexerBufferID != BufferID::Comment))
                {
                    
                    // Check if the buffer contains a token that can be part of a larger token
                    if (BiOpTokens.contains(Buffer))
                    {
                        // Check if it's compatible with the current characther 
                        std::string BufferAsString{Buffer};
                        if (BiOpTokens[BufferAsString] == Code[Char])
                        {
                            //cppcheck-suppress unassignedVariable
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
                    else if (UnOpTokens.contains(Buffer))
                    {
                        // Check if it's compatible with the current characther 
                        std::string BufferAsString{Buffer};
                        if (UnOpTokens[BufferAsString] == Code[Char])
                        {
                            //cppcheck-suppress unassignedVariable
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

                else if (LexerBufferID == BufferID::StringEnd) // NOLINTNEXTLINE
                {
                    LexerBufferID = BufferID::None; // resets BufferID when needed
                }

                else if 
                ((LexerBufferID == BufferID::Comment) &&
                ((Code[Char] == '\r') ||
                (Code[Char]  == ';')))
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

                    else if (std::find(Types.begin(), Types.end(), Buffer) != Types.end()) // if it is a keyword
                    {
                        Tokens.emplace_back(Token::TYPE_HINT, std::string{Buffer});
                    }

                    else if (std::find(Attributes.begin(), Attributes.end(), Buffer) != Attributes.end()) // if it is an attribute
                    {
                        Tokens.emplace_back(Token::ATTRIBUTE, std::string{Buffer});
                    }
                    
                    else if (std::find(Operators.begin(), Operators.end(), Buffer) != Operators.end()) // if it is a built in function
                    {
                        Tokens.emplace_back(Token::OPERATOR, std::string{Buffer});
                    }

                    // Floats 
                    else if 
                    ((Buffer.find_last_not_of("1234567890.") == std::string::npos) &&
                    (Buffer.find_first_of('.') != std::string::npos))
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
                    
                    case '\r':
                    {
                        Tokens.emplace_back(Token::NEWLINE, "\r");
                        ++StartIndex;
                        break;
                    }

                    case ';':
                    {
                        Tokens.emplace_back(Token::SEMICOLON, ";");
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

                    case ':':
                    {
                        Tokens.emplace_back(Token::COLON, ":");
                        break;
                    }

                    case '=':
                    {
                        Tokens.emplace_back(Token::EQUAL, "=");
                        break;
                    }
                    
                    case '^': // NOLINT
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
                        
                        if (StartIndex >= EndIndex) // The only way this can happen is when a "" occurs
                        {
                            Tokens.emplace_back(Token::STRING, "");
                            StartIndex = EndIndex;
                        }
                        else
                        {
                            Tokens.emplace_back(Token::STRING, Buffer);
                        }
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
    }
    return Tokens; 
}

bool Lexer::LexerCharCheck(char Char)
{
    return
    (Char   == ' ')   ||
    (Char   == ';')   ||
    (Char   == ':')   ||
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
    (Char   == '<')   ||
    (Char   == '\r');
}
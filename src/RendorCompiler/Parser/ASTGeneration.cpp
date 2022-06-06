#include "RendorCompiler/Parser/Parser.hpp"

void RendorParser::PrimaryParse()
{
    while (it != Tokens->end())
    {
        switch (CurrentToken)
        {
            case LexTok::NEWLINE:
            {
                ++CurrentLine;
                GetNextTok();
                break;
            }
            case LexTok::KEYWORD:
            {
                if (CurrentValue == "let")
                {
                    GetNextTok();
                    ParseIdentifer();
                }
                else if (CurrentValue == "edef")
                {
                    GetNextTok();
                    ParseEdef();
                }
                else 
                {
                    GetNextTok();
                }
                break;
            }
            case LexTok::IDENTIFIER:
            {
                ParseIdentifer();
                break;
            }
            default:
            {
                GetNextTok();
                break;
            }
        }
    }
}
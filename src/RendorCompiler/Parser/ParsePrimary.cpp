#include "RendorCompiler/Parser/Parser.hpp"

void RendorParser::PrimaryParse()
{
    while (it != Tokens->end())
    {
        switch (CurrentToken)
        {
            case LexTok::RBRACE:
            {
                return;
            }
            case LexTok::KEYWORD:
            {
                if (CurrentValue == "let")
                {
                    GetNextTok();
                    ParseIdentifer(std::string{CurrentValue});
                }
                else if (CurrentValue == "edef")
                {
                    GetNextTok();
                    ParseEdef(std::string{CurrentValue});
                }
                else if (CurrentValue == "import")
                {
                    GetNextTok();
                    ParseImport(std::string{CurrentValue});
                }
                break;
            }
            case LexTok::IDENTIFIER:
            {
                ParseIdentifer(std::string{CurrentValue});
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
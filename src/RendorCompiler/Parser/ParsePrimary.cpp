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
                GetNextTok();
                if (CurrentValue == "let")
                {
                    ParseIdentifer(std::string{CurrentValue});
                }
                else if (CurrentValue == "edef")
                {
                    ParseEdef(std::string{CurrentValue});
                }
                else if (CurrentValue == "import")
                {
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
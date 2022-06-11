#include "RendorCompiler/Parser/Parser.hpp"


void RendorParser::ParseEdef(const std::string& Identifier)
{
    auto Node = std::make_unique<nodes::FunctionDefinition>();
    Node->Name = std::string{Identifier};
    
    /* -------------------------------- Arguments ------------------------------- */
    GetNextTok();
    if (CurrentToken != LexTok::LPAREN)
    {
        throw error::CompilerRendorException(fmt::format("Err: Expected (, got {}; Line {}", 
            CurrentValue, CurrentLine), 
            CurrentLine);
    }

    do
    {
        GetNextTok();
        if (CurrentToken == LexTok::RPAREN)
        {
            break;
        }
        else 
        {
            ConstPtr Arg = ParseConstant();
            if (!Arg)
            {
                throw error::CompilerRendorException(fmt::format("Err: Expected an argument, got {}; Line {}", 
                    CurrentValue, CurrentLine), 
                    CurrentLine);
            }
            Node->Args.push_back(std::move(Arg));
        }
    } while (true);

    /* ---------------------------------- Type ---------------------------------- */
    GetNextTok();
    if (CurrentToken != LexTok::BIOP)
    {
        throw error::CompilerRendorException(fmt::format("Err: Expected -> operator, got {}; Line {}", 
            CurrentValue, CurrentLine), 
            CurrentLine);
    }
    if (CurrentValue != "->")
    {
        throw error::CompilerRendorException(fmt::format("Err: Expected -> operator, got {}; Line {}", 
            CurrentValue, CurrentLine), 
            CurrentLine);
    }
    GetNextTok();
    if (CurrentToken != LexTok::TYPE_HINT)
    {
        throw error::CompilerRendorException(fmt::format("Err: Expected type, got {}; Line {}", 
            CurrentValue, CurrentLine), 
            CurrentLine);
    }
    Node->RetType = Str_to_Type.at(CurrentValue);

    /* ---------------------------------- Body ---------------------------------- */
    GetNextTok();
    if (CurrentToken != LexTok::LBRACE)
    {
        throw error::CompilerRendorException(fmt::format("Err: Expected {{, got {}; Line {}", 
            CurrentValue, CurrentLine), 
            CurrentLine);
    }
    AddScope(&Node->FunctionBody.Nodes);
    PrimaryParse();
    RemoveScope();
    AddToMain(std::move(Node));
    GetNextTok();
}
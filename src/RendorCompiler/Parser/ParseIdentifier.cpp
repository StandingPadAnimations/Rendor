#include "RendorCompiler/Parser/Parser.hpp"
#include <fmt/color.h>
#include <fmt/core.h>
#include <memory>

void RendorParser::ParseIdentifer(const std::string& Identifier)
{
    GetNextTok();
    switch (CurrentToken)
    {
        case LexTok::COLON:
        {
            std::unique_ptr<nodes::Assignment> Node = std::make_unique<nodes::Assignment>();
            Node->Name = Identifier;
            GetNextTok();
            
            // If the user didn't provide a type
            if (CurrentToken != LexTok::TYPE_HINT)
            {
                throw error::CompilerRendorException(fmt::format("Err: Expected type, got {}; Line {}", 
                    CurrentValue, CurrentLine), 
                    CurrentLine);
            }

            // Store the type
            Node->ConstType = Str_to_Type.at(CurrentValue);
            const std::string_view Type_as_str = CurrentValue;

            GetNextTok();

            // If there isn't an equal sign
            if (CurrentToken != LexTok::EQUAL)
            {
                throw error::CompilerRendorException(fmt::format("Err: Expected =, got {}; Line {}", 
                    CurrentValue, CurrentLine), 
                    CurrentLine);
            }

            GetNextTok();
            ConstPtr Val = ParseConstant();
            if (!Val)
            {
                throw error::CompilerRendorException(fmt::format("Err: Expected a value, got {}; Line {}", 
                    CurrentValue, CurrentLine), 
                    CurrentLine);
            }
            else if (Val->Type_ir != Node->ConstType)
            {
                throw error::CompilerRendorException(fmt::format("Err: Expected a value of type {}, instead got {}; Line {}", 
                    Type_as_str, CurrentValue, CurrentLine), 
                    CurrentLine);
            }
            Node->Val = std::move(Val);

            Variables[Node->Name] = Node->ConstType; // add to vector for mangling
            AddToMain(std::move(Node));
            break;
        }
        case LexTok::LPAREN:
        {
            std::unique_ptr<nodes::FunctionCall> Node = std::make_unique<nodes::FunctionCall>();
            Node->Name = Identifier;
            
            // Adding arguments
            do
            {
                GetNextTok();
                if (CurrentToken == LexTok::RPAREN)
                {
                    break;
                }
                else 
                {
                    const ConstPtr Arg = ParseConstant();
                    if (!Arg)
                    {
                        throw error::CompilerRendorException(fmt::format("Err: Expected an argument, got {}; Line {}", 
                            CurrentValue, CurrentLine), 
                            CurrentLine);
                    }

                    if (Arg->Type == nodes::NodeType::VARIABLE_REFERENCE)
                    {
                        const auto& Arg_r = static_cast<nodes::Reference&>(*Arg);
                        const auto Arg_r_Type = Variables[Arg_r.Val];
                        Node->Name = fmt::format("{}{}_", Node->Name, Type_to_Str.at(Arg_r_Type));
                    }
                    else
                    {
                        Node->Name = fmt::format("{}{}_", Node->Name, Type_to_Str.at(Arg->Type_ir));
                    }
                    Node->Args.push_back(std::move(Arg));
                }
            } while (true);
            AddToMain(std::move(Node));
            break;
        }
        case LexTok::BIOP:
        {
            if (CurrentValue == "::")
            {
                GetNextTok();
                ParseIdentifer(fmt::format("{}::{}_", Identifier, CurrentValue));
                break;
            }
            FALLTHROUGH;
        }
        default:
        {
            throw error::CompilerRendorException(fmt::format("Err: Unexpected {} token; Line {}", CurrentValue, CurrentLine), CurrentLine);
        }
    }
}

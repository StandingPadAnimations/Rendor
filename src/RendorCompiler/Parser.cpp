#include "RendorCompiler/Parser.hpp"

/*----------------------------------------------------------------
In the parser loop(which streams tokens), please avoid using the continue keyword. In the past, it has caused unexpected behavior. The exception(s) are:
    * When you have to move on to the next token but know you may execute unwanted code(though using if-else is preferable here)

If you do use continue, please do:
    LastToken = token;
    LastValue = value;
    ++TokenIndex;
    continue;

This way, the parser works as expected

Also, use braces in each case of the switch statement like so:
case x:
{
    ...
}
break;

To avoid issues with scopes
----------------------------------------------------------------*/

std::string ByteCodeGen (const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::vector<std::string>& ByteCode, uint32_t& ByteCodeNumber);

std::vector<std::string> Parser (const std::vector<std::pair<Lex::Token, std::string>>& Tokens)
{
    // Node related stuff
    Main Script;
    std::vector<std::vector<std::unique_ptr<Node>>*> ScopeList {&Script.Global.ConnectedNodes}; // Scopes

    // Parser related stuff
    TempID ParserTempID = TempID::None;
    std::map<std::string, char> IdentifiersMap; // Defines identifiers
    uint32_t LineNumber = 1;

    uint32_t ScopeLevel = 0;

    bool IsScript = false;

    // Bytecode vector
    std::vector<std::string> ByteCode;
    
    for (auto const& [token, value] : Tokens)
    {
        std::cout << "Token: " << static_cast<std::underlying_type<lt>::type>(token) << " " << value << std::endl;
        
        std::vector<std::unique_ptr<Node>>* Scope = ScopeList.back();

        switch (token)
        {
            case lt::NEWLINE: // * Newlines
            {
                ++LineNumber;
                break; 
            }

            // Functions and variables
            case lt::IDENTIFIER: // * user defined things 
            {
                if 
                ((IdentifiersMap.find(value) == IdentifiersMap.end()) &&
                (ParserTempID == TempID::None)) // if it's a new variable 
                {
                    ParserTempID = TempID::IdentifierDefinition;
                    IdentifiersMap[value] = 'N';
                }

                else if (ParserTempID == TempID::FunctionDefiniton) // functions
                {
                    if (value == "main")
                    {
                        IsScript = true; // This is a script and not a library or module 
                    }
                    Scope->push_back(std::make_unique<Edef>(value));
                    IdentifiersMap[value] = 'F';
                }

                else if (ParserTempID == TempID::VariableDefition) // copying variables
                {
                    if (IdentifiersMap.find(value) == IdentifiersMap.end()) // if it's a new variable 
                    {
                        char Identifier = IdentifiersMap[value]; // retrive the current type of the variable being copied
                        auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                        IdentifiersMap[AssignmentNode.VariableName] = Identifier;

                        AssignmentNode.Value = (boost::format("_&%s") % value).str();
                    }
                    else // if variable does not exist 
                    {
                        throw error::RendorException((boost::format("Identifier %s not defined; Line %s") % value % LineNumber).str());
                    }
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.emplace_back((boost::format("_&%s") % value).str()); // Add argument to Node
                }

                
                else if (ParserTempID == TempID::FunctionArgumentsDefinition) // Function arguments 
                {
                    auto& EdefNode = dynamic_cast<Edef&>(*Scope->back());
                    EdefNode.Args.emplace_back(value); // Add argument to Node
                }
                break;
            }

            case lt::BUILT_IN_FUNCTION: // * built in functions
            {
                Scope->push_back(std::make_unique<FunctionCall>(value));
                ParserTempID = TempID::FunctionCall;
                break;
            }

            // Symbols
            case lt::EQUAL: // * = sign 
            {
                if (ParserTempID == TempID::IdentifierDefinition)
                {
                    ParserTempID = TempID::VariableDefition;
                    Scope->push_back(std::make_unique<AssignVariable>(std::prev(IdentifiersMap.end())->first));
                }
                else 
                {
                    throw error::RendorException((boost::format("Random = found on line %s") % LineNumber).str());
                }
                break;
            }

            case lt::LPAREN: // * ( sign 
            {
                if (ParserTempID == TempID::FunctionCall)
                {
                    // Do nothing 
                }
                if (ParserTempID == TempID::FunctionDefiniton) // Defining a funcion 
                {
                    ParserTempID = TempID::FunctionArgumentsDefinition;
                }
                break;
            }

            case lt::RPAREN: // * ) sign
            {
                if (ParserTempID == TempID::FunctionArgumentsDefinition)
                {
                    ParserTempID = TempID::FunctionScope;
                }
                break;
            }

            case lt::LBRACE: // * { sign
            {
                if (ParserTempID == TempID::FunctionScope)
                {
                    ParserTempID = TempID::None;
                    ++ScopeLevel;

                    // Add it as a scope
                    auto& EdefNode = dynamic_cast<Edef&>(*Scope->back());
                    ScopeList.emplace_back(EdefNode.Body);
                }
                break;
            }

            case lt::RBRACE: // * } sign
            {
                --ScopeLevel;
                ScopeList.pop_back();
                break;
            }

            // Types
            case lt::INT: 
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    char Identifier = 'I';
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                    AssignmentNode.Value = value;
                    IdentifiersMap[AssignmentNode.VariableName] = Identifier;
                    AssignmentNode.VariableType = VariableTypes::Int;
                }
                
                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.emplace_back(value); // Add argument to Node
                }
                break;
            }

            case lt::FLOAT:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    char Identifier = 'D';
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                    AssignmentNode.Value = value;
                    IdentifiersMap[AssignmentNode.VariableName] = Identifier;
                    AssignmentNode.VariableType = VariableTypes::Float;
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.emplace_back(value); // Add argument to Node
                }
                break;
            }

            case lt::STRING:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    char Identifier = 'S';
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                    AssignmentNode.Value = value;
                    IdentifiersMap[AssignmentNode.VariableName] = Identifier;
                    AssignmentNode.VariableType = VariableTypes::String;
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.emplace_back(value); // Add argument to Node
                }
                break;
            }
                
            case lt::BOOL:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    char Identifier = 'B';
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                    AssignmentNode.Value = value;
                    IdentifiersMap[AssignmentNode.VariableName] = Identifier;
                    AssignmentNode.VariableType = VariableTypes::Bool;
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.emplace_back(value); // Add argument to Node
                }
                break;
            }

            case lt::KEYWORD: // * keywords
            {
                if (value == "edef")
                {
                    ParserTempID = TempID::FunctionDefiniton;
                }
                else 
                {
                    throw error::RendorException("Not supported yet");
                }
                break;
            }

            default:
                throw error::RendorException("Not supported yet");
        }
    }
    

    //BYTECODE GENERATION-----------------------------------------------------------------------------------------------------

    uint32_t ByteCodeNumber = 0;

    if (!IsScript)
    {
        ByteCode.emplace_back((boost::format("%s NOT_SCRIPT TRUE") % ByteCodeNumber).str());
        ++ByteCodeNumber;
    }

    ByteCode.emplace_back((boost::format("%s LOAD 0") % ByteCodeNumber).str()); // For Global Scope
    ++ByteCodeNumber;

    for (const auto& Node : (*Script.GlobalBody))
    {
        ByteCode.emplace_back((boost::format("%s %s") % ByteCodeNumber % ByteCodeGen(Node->Type(), Node, ByteCode, ByteCodeNumber)).str());
        ++ByteCodeNumber;
    }

    ByteCode.emplace_back("0 END 0"); // End Global Scope

    return ByteCode;
}

std::string ByteCodeGen(const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::vector<std::string>& ByteCode, uint32_t& ByteCodeNumber)
{

    if (ClassType == NodeType::AssignVariable) 
    {
        auto& AssignmentNode = static_cast<AssignVariable&>(*NodeClass); 
        int Type;

        switch (AssignmentNode.VariableType)
        {
            case VariableTypes::Int:
                Type = 0;
                break;
            
            case VariableTypes::Float:
                Type = 1;
                break;

            case VariableTypes::String:
                Type = 2;
                break;

            case VariableTypes::Bool:
                Type = 3;
                break;

            case VariableTypes::Arith: 
                throw error::RendorException("Arithmethic not supported yet");

            default:
                throw error::RendorException("Invalid node type; Assignment Variable Fail");
        }

        ByteCode.emplace_back((boost::format("%s CONST %s %s") % ByteCodeNumber % Type % AssignmentNode.Value).str());

        ++ByteCodeNumber;

        return (boost::format("ASSIGN %s") % AssignmentNode.VariableName).str();
    }

    else if (ClassType == NodeType::Edef)
    {
        auto& EdefNode = static_cast<Edef&>(*NodeClass); 

        ByteCode.emplace_back((boost::format("%s DEFINE %s") % ByteCodeNumber % EdefNode.Name).str());
        ++ByteCodeNumber;

        for (const auto& Arg : EdefNode.Args) // Arguments 
        {
            ByteCode.emplace_back((boost::format("%s ARGUMENT %s") % ByteCodeNumber % Arg).str());
            ++ByteCodeNumber;
        }

        for (const auto& Node : (*EdefNode.Body)) // actual body 
        {
            ByteCode.emplace_back((boost::format("%s %s") % ByteCodeNumber % ByteCodeGen(Node->Type(), Node, ByteCode, ByteCodeNumber)).str());
            ++ByteCodeNumber;
        }

        return "FUNCTION END";
    }

    else if (ClassType == NodeType::FunctionCall)
    {
        auto& CallNode = static_cast<FunctionCall&>(*NodeClass); 

        ByteCode.emplace_back((boost::format("%s CALL %s") % ByteCodeNumber % CallNode.Function).str());
        ++ByteCodeNumber;

        for (const auto& Arg : CallNode.Args) // Arguments 
        {
            ByteCode.emplace_back((boost::format("%s CALL_ARG %s") % ByteCodeNumber % Arg).str());
            ++ByteCodeNumber;
        }

        return (boost::format("FINALIZE_CALL %s") % CallNode.Function).str();
    }

    return "ERROR";
}

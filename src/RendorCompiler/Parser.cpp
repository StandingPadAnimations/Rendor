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

/* -------------------------------------------------------------------------- */
/*                             Foward declarations                            */
/* -------------------------------------------------------------------------- */
std::string ByteCodeGen (const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::vector<std::string>& ByteCode, uint32_t& ByteCodeNumber);


/* -------------------------------------------------------------------------- */
/*                               Parser function                              */
/* -------------------------------------------------------------------------- */
std::vector<std::string> Parser (const std::vector<std::pair<Lex::Token, std::string>>& Tokens)
{
    // Node related stuff
    Main Script;
    std::vector<std::vector<std::unique_ptr<Node>>*> ScopeList {&Script.Global.ConnectedNodes}; // Scopes

    // Parser related stuff
    TempID ParserTempID = TempID::None;
    std::map<std::string, char> IdentifiersMap; // Defines identifiers
    std::string LastIdentifier;
    uint32_t LineNumber = 1;

    uint32_t ScopeLevel = 0;

    bool IsScript = false;

    // Bytecode vector
    std::vector<std::string> ByteCode;
    
    for (auto const& [token, value] : Tokens)
    {        
        std::vector<std::unique_ptr<Node>>* Scope = ScopeList.back();
        // std::cout << "Token: " << static_cast<std::underlying_type<Lex::Token>::type>(token) << " " << value << std::endl;

        switch (token)
        {
            case lt::NEWLINE: // * Newlines
            {
                if (ParserTempID != TempID::FunctionScope)
                {
                    ParserTempID = TempID::None;
                }

                ++LineNumber;
                break; 
            }

            /* -------------------------------------------------------------------------- */
            /*                           Functions and variables                          */
            /* -------------------------------------------------------------------------- */
            case lt::IDENTIFIER: // * user defined things 
            {
                /* ------------ if found during definition of another identifier ------------ */
                if (ParserTempID == TempID::IdentifierDefinition)
                {
                    throw error::RendorException((boost::format("Syntax Error: %s found during the definition of %s; Line %s") % value % LastIdentifier % LineNumber).str());
                }
                

                /* -------------------------------- functions ------------------------------- */
                else if 
                ((IdentifiersMap.find(value) != IdentifiersMap.end()) &&
                (IdentifiersMap.at(value) == 'F'))
                {
                    Scope->push_back(std::make_unique<FunctionCall>(LastIdentifier));
                    ParserTempID = TempID::FunctionCall;
                }
                
                /* -------------------------------- not known ------------------------------- */
                else if 
                (((IdentifiersMap.find(value) == IdentifiersMap.end()) ||
                ((IdentifiersMap.find(value) != IdentifiersMap.end()) &&
                (IdentifiersMap.at(value) != 'F'))) &&
                (ParserTempID == TempID::None)) // if it's a new variable 
                {
                    ParserTempID = TempID::IdentifierDefinition;
                    IdentifiersMap[value] = 'N';
                    LastIdentifier = value;
                }
                /* --------------------------- defining functions --------------------------- */
                else if (ParserTempID == TempID::FunctionDefiniton) // functions
                {
                    if (IdentifiersMap.find(value) != IdentifiersMap.end())
                    {
                        throw error::RendorException((boost::format("Redefinition Error: %s is already defined!; Line %s") % value % LineNumber).str());
                    }
                    else if (value == "main")
                    {
                        IsScript = true; // This is a script and not a library or module 
                    }
                    Scope->push_back(std::make_unique<Edef>(value));
                    IdentifiersMap[value] = 'F';
                }

                /* ---------------------------- copying variables --------------------------- */
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
                        throw error::RendorException((boost::format("Identifier Error: Identifier %s not defined; Line %s") % value % LineNumber).str());
                    }
                }

                /* -------------------------- if it is an argument -------------------------- */
                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());

                    if (IdentifiersMap.find(value) == IdentifiersMap.end())
                    {
                        throw error::RendorException((boost::format("Identifier Error: %s does not exist! Line %s") % value % LineNumber).str());
                    }

                    switch (IdentifiersMap[value])
                    {
                        case 'I':
                        {
                            FunctionNode.Args.emplace_back(0, (boost::format("_&%s") % value).str()); // Add argument to Node
                            break;
                        }

                        case 'D':
                        {
                            FunctionNode.Args.emplace_back(1, (boost::format("_&%s") % value).str()); // Add argument to Node
                            break;
                        }

                        case 'S':
                        {
                            FunctionNode.Args.emplace_back(2, (boost::format("_&%s") % value).str()); // Add argument to Node
                            break;
                        }

                        case 'B':
                        {
                            FunctionNode.Args.emplace_back(3, (boost::format("_&%s") % value).str()); // Add argument to Node
                            break;
                        }

                        case 'N':
                        {
                            FunctionNode.Args.emplace_back(-1, (boost::format("_&%s") % value).str()); // Add argument to Node
                            break;
                        }
                    }
                }

                /* ------------- if it's a scoped argument(like edef lol(world)) ------------ */
                else if (ParserTempID == TempID::FunctionArgumentsDefinition) // Function arguments 
                {
                    auto& EdefNode = dynamic_cast<Edef&>(*Scope->back());
                    EdefNode.Args.emplace_back(value); // Add argument to Node
                    IdentifiersMap[value] = 'N';
                }
                break;
            }

            /* -------------------------------------------------------------------------- */
            /*                             Built in functions                             */
            /* -------------------------------------------------------------------------- */
            case lt::BUILT_IN_FUNCTION: 
            {
                if(ParserTempID == TempID::VariableDefition)
                {
                    char Identifier = 'I';
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                    AssignmentNode.Value = value;
                    IdentifiersMap[AssignmentNode.VariableName] = Identifier;
                    AssignmentNode.VariableType = VariableTypes::Function;
                }
                Scope->push_back(std::make_unique<FunctionCall>(value));
                ParserTempID = TempID::FunctionCall;
                break;
            }

            /* -------------------------------------------------------------------------- */
            /*                                   Symbols                                  */
            /* -------------------------------------------------------------------------- */

            /* ---------------------------------- Equal --------------------------------- */
            case lt::EQUAL: // * = sign 
            {
                if (ParserTempID == TempID::IdentifierDefinition)
                {
                    ParserTempID = TempID::VariableDefition;
                    Scope->push_back(std::make_unique<AssignVariable>(LastIdentifier));
                }
                else 
                {
                    throw error::RendorException((boost::format("Syntax Error: = found on line %s, expected variable definition") % LineNumber).str());
                }
                break;
            }

            /* ---------------------------------- Comma --------------------------------- */
            case lt::COMMA:
            {
                if (ParserTempID == TempID::FunctionCall)
                {
                    // Do nothing 
                }

                else if (ParserTempID == TempID::FunctionArgumentsDefinition) // Defining a funcion 
                {
                    // Do nothing
                }
                break;
            }

            /* --------------------------------- L paren -------------------------------- */
            case lt::LPAREN: // * ( sign 
            {
                if (ParserTempID == TempID::FunctionCall)
                {
                    // Do nothing 
                }
                else if (ParserTempID == TempID::FunctionDefiniton) // Defining a funcion 
                {
                    ParserTempID = TempID::FunctionArgumentsDefinition;
                }
                else if (ParserTempID == TempID::IdentifierDefinition)
                {
                    Scope->push_back(std::make_unique<FunctionCall>(LastIdentifier));
                    ParserTempID = TempID::FunctionCall;
                    IdentifiersMap.erase(LastIdentifier);
                }
                break;
            }

            /* --------------------------------- R paren -------------------------------- */
            case lt::RPAREN: // * ) sign
            {
                if (ParserTempID == TempID::FunctionArgumentsDefinition)
                {
                    ParserTempID = TempID::FunctionScope;
                }
                else if (ParserTempID == TempID::FunctionCall)
                {
                    ParserTempID = TempID::None;
                }
                break;
            }

            /* --------------------------------- L brace -------------------------------- */
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

                else {
                    throw error::RendorException((boost::format("Syntax Error: { found outside of function scope; Line %s") % LineNumber).str());
                }
                break;
            }

            /* --------------------------------- R brace -------------------------------- */
            case lt::RBRACE: // * } sign
            {
                --ScopeLevel;
                ScopeList.pop_back();
                break;
            }

            /* -------------------------------------------------------------------------- */
            /*                                    Types                                   */
            /* -------------------------------------------------------------------------- */

            /* ----------------------------------- int ---------------------------------- */
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
                    FunctionNode.Args.emplace_back(0, value); // Add argument to Node
                }
                break;
            }

            /* ---------------------------------- float --------------------------------- */
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
                    FunctionNode.Args.emplace_back(1, value); // Add argument to Node
                }
                break;
            }

            /* --------------------------------- string --------------------------------- */
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
                    FunctionNode.Args.emplace_back(2, value); // Add argument to Node
                }
                break;
            }
            
            /* ---------------------------------- bool ---------------------------------- */
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
                    FunctionNode.Args.emplace_back(3, value); // Add argument to Node
                }
                break;
            }

            /* -------------------------------------------------------------------------- */
            /*                                  Keywords                                  */
            /* -------------------------------------------------------------------------- */
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

            /* -------------------------------------------------------------------------- */
            /*                           Anything not supported                           */
            /* -------------------------------------------------------------------------- */
            default:
                throw error::RendorException("Not supported yet");
        }
    }
    

    /* -------------------------------------------------------------------------- */
    /*                             ByteCode generation                            */
    /* -------------------------------------------------------------------------- */

    std::cout << "Generating bytecode..." << std::endl;

    uint32_t ByteCodeNumber = 0;
    
    /* ----------------------------- Is it a script ----------------------------- */
    if (!IsScript)
    {
        ByteCode.emplace_back((boost::format("%s NOT_SCRIPT TRUE") % ByteCodeNumber).str());
        ++ByteCodeNumber;
    }

    /* -------------------------- Loading global scope -------------------------- */
    ByteCode.emplace_back((boost::format("%s LOAD 0") % ByteCodeNumber).str()); 
    ++ByteCodeNumber;

    /* ---------------- Generating the bytecode from the AST tree --------------- */
    for (const auto& Node : (*Script.GlobalBody))
    {
        ByteCode.emplace_back((boost::format("%s %s") % ByteCodeNumber % ByteCodeGen(Node->Type(), Node, ByteCode, ByteCodeNumber)).str());
        ++ByteCodeNumber;
    }

    /* ------------------------- ending the global scope ------------------------ */
    ByteCode.emplace_back("0 END 0"); 

    /* ----------------------------- return bytecode ---------------------------- */
    return ByteCode;
}


/* -------------------------------------------------------------------------- */
/*                      Bytecode Generation loop function                     */
/* -------------------------------------------------------------------------- */

std::string ByteCodeGen(const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::vector<std::string>& ByteCode, uint32_t& ByteCodeNumber)
{

    /* -------------------------------------------------------------------------- */
    /*                             assigning variables                            */
    /* -------------------------------------------------------------------------- */
    if (ClassType == NodeType::AssignVariable) 
    {
        auto& AssignmentNode = static_cast<AssignVariable&>(*NodeClass); 
        int Type;

        /* ------------------------------- Check type ------------------------------- */
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

            case VariableTypes::Function:
                Type = 4; 
                break;

            case VariableTypes::Arith: 
                throw error::RendorException("Arithmethic not supported yet");

            default:
                throw error::RendorException("Invalid node type; Assignment Variable Fail");
        }

        /* ---------------------------- generate bytecode --------------------------- */
        if (AssignmentNode.Value.size())
        {
            ByteCode.emplace_back((boost::format("%s CONST %s %s") % ByteCodeNumber % Type % AssignmentNode.Value).str());
        }

        ++ByteCodeNumber;

        return (boost::format("ASSIGN %s") % AssignmentNode.VariableName).str();
    }

    /* -------------------------------------------------------------------------- */
    /*                             defining functions                             */
    /* -------------------------------------------------------------------------- */
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

        for (const auto& [type, arg] : CallNode.Args) // Arguments 
        {
            ByteCode.emplace_back((boost::format("%s CALL_ARG %s %s") % ByteCodeNumber % type % arg).str());
            ++ByteCodeNumber;
        }

        return (boost::format("FINALIZE_CALL %s") % CallNode.Function).str();
    }

    return "ERROR";
}

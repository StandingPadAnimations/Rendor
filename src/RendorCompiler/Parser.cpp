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
static std::string ByteCodeGen (const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::vector<std::string>& ByteCode);
static void DeltaOptimizer (std::vector<std::string>& ByteCode);


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
    std::string LastOp = "";
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
                if 
                ((ParserTempID != TempID::FunctionScope) &&
                (ParserTempID != TempID::IfElseScope))
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
                ((IdentifiersMap.contains(value)) &&
                (IdentifiersMap.at(value) == 'F'))
                {
                    Scope->push_back(std::make_unique<FunctionCall>(LastIdentifier));
                    ParserTempID = TempID::FunctionCall;
                }
                
                /* -------------------------------- not known ------------------------------- */
                else if 
                (((!IdentifiersMap.contains(value)) ||
                ((IdentifiersMap.contains(value))   &&
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
                    if (IdentifiersMap.contains(value))
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
                    if (IdentifiersMap.contains(value)) // if it's a new variable 
                    {
                        auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
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

                    if (!IdentifiersMap.contains(value))
                    {
                        throw error::RendorException((boost::format("Identifier Error: %s does not exist! Line %s") % value % LineNumber).str());
                    }

                    FunctionNode.Args.emplace_back((boost::format("_&%s") % value).str()); // Add argument to Node
                }

                /* ------------- if it's a scoped argument(like edef lol(world)) ------------ */
                else if (ParserTempID == TempID::FunctionArgumentsDefinition) // Function arguments 
                {
                    auto& EdefNode = dynamic_cast<Edef&>(*Scope->back());
                    EdefNode.Args.emplace_back(value); // Add argument to Node
                    IdentifiersMap[value] = 'N';
                }

                /* ------------------------------ if statements ----------------------------- */
                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    IfElseNode.Conditions.emplace_back("_&" + value); // Add condition 
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
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                    AssignmentNode.Value = "_$" + value;
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

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    
                    if (LastOp == "=")
                    {
                        IfElseNode.Conditions.emplace_back((boost::format("%s=") % LastOp).str());
                        LastOp = "";
                    }
                    else {
                        LastOp = value;
                    }
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

                else if (ParserTempID == TempID::IfStatementDefinition)
                {
                    ParserTempID = TempID::ConditionDefinition;
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

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    ParserTempID = TempID::IfElseScope;
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

                else if (ParserTempID == TempID::IfElseScope)
                {
                    ParserTempID = TempID::None;
                    ++ScopeLevel;

                    // Add it as a scope
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    ScopeList.emplace_back(IfElseNode.Body);
                }

                else {
                    throw error::RendorException((boost::format("Syntax Error: { found outside of function/if/else scope; Line %s") % LineNumber).str());
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
                    FunctionNode.Args.emplace_back(value); // Add argument to Node
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    IfElseNode.Conditions.emplace_back(value); // Add condition 
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
                    FunctionNode.Args.emplace_back(value); // Add argument to Node
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    IfElseNode.Conditions.emplace_back(value); // Add condition 
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
                    AssignmentNode.Value = "_S" + value;
                    IdentifiersMap[AssignmentNode.VariableName] = Identifier;
                    AssignmentNode.VariableType = VariableTypes::String;
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.emplace_back("_S" + value); // Add argument to Node
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    IfElseNode.Conditions.emplace_back("_S" + value); // Add condition 
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
                    FunctionNode.Args.emplace_back(value); // Add argument to Node
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    IfElseNode.Conditions.emplace_back(value); // Add condition 
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

                else if 
                ((value == "if") ||
                (value == "else"))
                {
                    Scope->push_back(std::make_unique<IfElse>());
                    ParserTempID = TempID::IfStatementDefinition;
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
    /* ----------------------------- Is it a script ----------------------------- */
    if (!IsScript)
    {
        ByteCode.emplace_back("NOT_SCRIPT TRUE");
    }
    /* -------------------------- Loading global scope -------------------------- */
    ByteCode.emplace_back("LOAD 0");

    /* ---------------- Generating the bytecode from the AST tree --------------- */
    for (const auto& Node : (*Script.GlobalBody))
    {
        ByteCode.emplace_back(ByteCodeGen(Node->Type(), Node, ByteCode));
    }
    /* ------------------------- ending the global scope ------------------------ */
    ByteCode.emplace_back("END 0"); 
    /* ---------------------------- Optimize bytecode --------------------------- */
    DeltaOptimizer(ByteCode);
    /* ----------------------------- return bytecode ---------------------------- */
    return ByteCode;
}


/* -------------------------------------------------------------------------- */
/*                      Bytecode Generation loop function                     */
/* -------------------------------------------------------------------------- */

static std::string ByteCodeGen(const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::vector<std::string>& ByteCode)
{

    /* -------------------------------------------------------------------------- */
    /*                             assigning variables                            */
    /* -------------------------------------------------------------------------- */
    if (ClassType == NodeType::AssignVariable) 
    {
        auto& AssignmentNode = static_cast<AssignVariable&>(*NodeClass); 

        /* ---------------------------- generate bytecode --------------------------- */
        if (AssignmentNode.Value.size())
        {
            ByteCode.emplace_back((boost::format("CONST %s") % AssignmentNode.Value).str());
        }
        return (boost::format("ASSIGN %s") % AssignmentNode.VariableName).str();
    }

    /* -------------------------------------------------------------------------- */
    /*                             defining functions                             */
    /* -------------------------------------------------------------------------- */
    else if (ClassType == NodeType::Edef)
    {
        auto& EdefNode = static_cast<Edef&>(*NodeClass); 

        ByteCode.emplace_back((boost::format("DEFINE %s") % EdefNode.Name).str());

        for (std::vector<std::string>::reverse_iterator Arg = EdefNode.Args.rbegin(); Arg != EdefNode.Args.rend(); ++Arg) // Arguments 
        {
            ByteCode.emplace_back((boost::format("ARGUMENT %s") % *Arg).str());
        }

        for (const auto& Node : (*EdefNode.Body)) // actual body 
        {
            ByteCode.emplace_back(ByteCodeGen(Node->Type(), Node, ByteCode));
        }

        return "FUNCTION END";
    }

    else if (ClassType == NodeType::FunctionCall)
    {
        auto& CallNode = static_cast<FunctionCall&>(*NodeClass); 

        ByteCode.emplace_back((boost::format("CALL %s") % CallNode.Function).str());

        for (const auto& arg : CallNode.Args) // Arguments 
        {
            ByteCode.emplace_back((boost::format("CALL_ARG %s") % arg).str());

        }

        return (boost::format("FINALIZE_CALL %s") % CallNode.Function).str();
    }

    /* -------------------------------------------------------------------------- */
    /*                         Defining if-else statements                        */
    /* -------------------------------------------------------------------------- */
    else if (ClassType == NodeType::IfElse)
    {
        auto& IfElseNode = static_cast<IfElse&>(*NodeClass); 

        /* --------------------------- Generate Conditions -------------------------- */
        std::string * Conditon1 = &IfElseNode.Conditions[0];
        std::string * Conditon2 = &IfElseNode.Conditions[2];
        std::string * Op        = &IfElseNode.Conditions[1];

        ByteCode.emplace_back((boost::format("CONST %s") % (*Conditon1)).str());
        ByteCode.emplace_back((boost::format("CONST %s") % (*Conditon2)).str());

        if ((*Op) == "==")
        {
            ByteCode.emplace_back("OPERATOR EQUAL");
        }
        else if ((*Op) == "!=")
        {
            ByteCode.emplace_back("OPERATOR NOT_EQUAL");
        }
        else if ((*Op) == ">")
        {
            ByteCode.emplace_back("OPERATOR GREATER_THAN");
        }
        else if ((*Op) == "<")
        {
            ByteCode.emplace_back("OPERATOR LESS_THAN");
        }
        else if ((*Op) == ">=")
        {
            ByteCode.emplace_back("OPERATOR GREATER_OR_EQUAL");
        }
        else if ((*Op) == "<=")
        {
            ByteCode.emplace_back("OPERATOR LESS_OR_EQUAL");
        }
        ByteCode.emplace_back("JMP_IF_FALSE ");
        
        /* -------------------- For the JMP_IF_FALSE instruction -------------------- */
        size_t IndexOfJMP = ByteCode.size() - 1;
        
        for (const auto& Node : (*IfElseNode.Body)) // actual body 
        {
            ByteCode.emplace_back(ByteCodeGen(Node->Type(), Node, ByteCode));
        }

        ByteCode[IndexOfJMP] += std::to_string(ByteCode.size() - 1);
        return "ENDIF STATE";
    }

    return "ERROR";
}

static void DeltaOptimizer(std::vector<std::string>& ByteCode)
{
    std::string SavedString = "";
    for (auto ByteCodeOp = ByteCode.begin(); ByteCodeOp != ByteCode.end();)
    {
        size_t ByteCodeSpaceIndex  = ByteCodeOp->find_first_of(" ");
        std::string_view Command  (ByteCodeOp->begin(), ByteCodeOp->begin() + ByteCodeSpaceIndex);
        std::string_view Args     (ByteCodeOp->begin() + (ByteCodeSpaceIndex + 1), ByteCodeOp->end());

        if (Command == "CONST")
        {
            if 
            ((Args[0] == '_') &&
            (Args[1] == '$'))
            {
                SavedString = *(ByteCodeOp + 1);
                
                // Erase const instruction 
                ByteCode.erase(ByteCodeOp);

                // Erase assign instruction
                ByteCode.erase(ByteCodeOp);
            }
        }
        else if (Command == "FINALIZE_CALL")
        {
            if (SavedString.size())
            {
                ByteCode.insert(ByteCodeOp + 1, SavedString);
                SavedString = "";
            }
        }
        ++ByteCodeOp;
    }
}

#include "RendorCompiler/Parser/Parser.hpp"

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
    break;
}

To avoid issues with scopes
----------------------------------------------------------------*/

/* -------------------------------------------------------------------------- */
/*                               Parser function                              */
/* -------------------------------------------------------------------------- */
std::vector<std::string> Parser::ASTGeneration(const std::vector<std::pair<Lex::Token, std::string>>& Tokens)
{
    for (auto const& [token, value] : Tokens)
    {        
        Scope = ScopeList.back();
        ParserTempID = ParserTempIDList.back();
        // std::cout << "Token: " << static_cast<std::underlying_type<Lex::Token>::type>(token) << " " << value << std::endl;

        switch (token)
        {
            case lt::NEWLINE: // * Newlines
            {
                if 
                ((ParserTempID != TempID::FunctionScope) &&
                (ParserTempID != TempID::IfElseScope))
                {
                    if (ParserTempIDList.size() == 2)
                    {
                        PopTempID();
                    }
                    else
                    {
                        AddTempID(TempID::None);
                    }
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
                    if (GetTypeOfNode() == NodeType::AssignVariable)
                    {
                        /* ------------------------ Create FunctionCall Node ------------------------ */
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        throw error::RendorException((boost::format("Syntax Error: %s found during the definition of %s; Line %s") % value % AssignVariableNode.VariableName % LineNumber).str());
                    }
                }

                /* -------------------------- function definitions -------------------------- */
                else if (ParserTempID == TempID::FunctionDefiniton) // functions
                {
                    if (value == "main")
                    {
                        IsScript = true; // This is a script and not a library or module 
                    }
                    PushToScope(std::make_unique<Edef>(value, LineNumber));
                }

                /* -------------------------- if it is an argument -------------------------- */
                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    PushToScope(std::make_unique<Reference>(value, LineNumber)); // Add argument to Node
                }

                else if (ParserTempID == TempID::FowardDefinition)
                {
                    PushToScope(std::make_unique<FowardEdef>(value, LineNumber)); // Add argument to Node
                }

                /* ------------- if it's a scoped argument(like edef lol(world)) ------------ */
                else if (ParserTempID == TempID::FunctionArgumentsDefinition) // Function arguments 
                {
                    if (GetTypeOfNode() == NodeType::Edef)
                    {
                        auto& EdefNode = static_cast<Edef&>(*Scope->back());
                        EdefNode.Args.emplace_back(value, NodeType::Any); // Add argument to Node
                    }
                }

                else if (ParserTempID == TempID::FowardArgsDefinition)
                {
                    if (GetTypeOfNode() == NodeType::FowardEdef)
                    {
                        auto& FowardNode = static_cast<FowardEdef&>(*Scope->back());
                        FowardNode.Args.emplace_back(value, NodeType::Any); // Add argument to Node
                    }
                }

                /* ------------------------------ if statements ----------------------------- */
                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    AddTokenToConditions<IfElse, Reference>(IfElseNode, value, LineNumber);
                }
                
                /* ----------------------------- Add as variable ---------------------------- */
                else 
                {
                    PushToScope(std::make_unique<Reference>(value, LineNumber));
                    AddTempID(TempID::IdentifierDefinition);
                }
                break;
            }

            /* -------------------------------------------------------------------------- */
            /*                             Built in functions                             */
            /* -------------------------------------------------------------------------- */
            case lt::BUILT_IN_FUNCTION: 
            {
                /* ------------------------- Functions to variables ------------------------- */
                if(ParserTempID == TempID::VariableDefition)
                {
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                    AssignmentNode.Value = std::make_unique<FunctionCall>(value, LineNumber);
                }

                /* ---------------- Functions on their own or with functions ---------------- */
                else
                {
                    PushToScope(std::make_unique<FunctionCall>(value, LineNumber));
                    auto& FunctionNode = static_cast<FunctionCall&>(*Scope->back());
                    AddScope(&FunctionNode.Args);
                }
                AddTempID(TempID::FunctionCall);
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
                    auto& ReferenceNode = static_cast<Reference&>(*Scope->back());
                    ReplaceNode(std::make_unique<AssignVariable>(ReferenceNode.Value, LineNumber)); 
                    AddTempID(TempID::VariableDefition); 
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
                /* -------------------- function calls in function calls -------------------- */
                if (ParserTempID == TempID::FunctionCall)
                {
                    if (Scope->size())
                    {
                        switch (GetTypeOfNode())
                        {
                            case NodeType::FunctionCall:
                            {
                                auto& FunctionCallNode = static_cast<FunctionCall&>(*Scope->back());
                                if (*Scope != FunctionCallNode.Args)
                                {
                                    AddScope(&FunctionCallNode.Args);
                                }
                                break;
                            }

                            case NodeType::AssignVariable:
                            {
                                auto& AssignmentNode = static_cast<AssignVariable&>(*Scope->back());
                                auto& FunctionCallNode = static_cast<FunctionCall&>(*AssignmentNode.Value);
                                if (*Scope != FunctionCallNode.Args)
                                {
                                    AddScope(&FunctionCallNode.Args);
                                }
                                break;
                            }

                            default:
                            {
                                break;
                            }
                        }
                    }
                }

                /* --------------------------- Defining functions --------------------------- */
                else if (ParserTempID == TempID::FunctionDefiniton) // Defining a funcion 
                {
                    PopTempID();
                    AddTempID(TempID::FunctionArgumentsDefinition);
                }

                else if (ParserTempID == TempID::FowardDefinition)
                {
                    PopTempID();
                    AddTempID(TempID::FowardArgsDefinition);
                }
                
                /* -------------------- Function Calls in the main scope -------------------- */
                else if (ParserTempID == TempID::IdentifierDefinition)
                {
                    /* ---------------- Check if the last node is an assign node ---------------- */
                    if (GetTypeOfNode() == NodeType::Reference)
                    {
                        /* ------------------------ Create FunctionCall Node ------------------------ */
                        auto& AssignVariableNode = static_cast<Reference&>(*Scope->back());
                        std::unique_ptr<FunctionCall> FunctionCallNode = std::make_unique<FunctionCall>(AssignVariableNode.Value, LineNumber);
                        
                        /* ------------------------------ Replace Node ------------------------------ */
                        ReplaceNode(std::move(FunctionCallNode));

                        /* ------------------------- Add arguments to scopes ------------------------ */
                        auto& FunctionCallNode2 = static_cast<FunctionCall&>(*Scope->back());
                        AddScope(&FunctionCallNode2.Args);
                        PopTempID();
                        AddTempID(TempID::FunctionCall);
                    }
                }

                /* ------------------------- Defining if statements ------------------------- */
                else if (ParserTempID == TempID::IfStatementDefinition)
                {
                    PopTempID();
                    AddTempID(TempID::ConditionDefinition);
                }

                /* ------------------------------- Arithmethic ------------------------------ */
                else if (ParserTempID == TempID::VariableDefition)
                {
                    if (GetTypeOfNode() == NodeType::AssignVariable)
                    {
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        AddToArithmethicNode(AssignVariableNode, value, LineNumber);
                    }
                }
                break;
            }

            /* --------------------------------- R paren -------------------------------- */
            case lt::RPAREN: // * ) sign
            {
                if (ParserTempID == TempID::FunctionArgumentsDefinition)
                {
                    PopTempID();
                    AddTempID(TempID::FunctionScope);
                }

                else if (ParserTempID == TempID::FowardArgsDefinition)
                {
                    PopTempID();
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    PopTempID();
                    AddTempID(TempID::IfElseScope);
                }

                else if (ParserTempID == TempID::FunctionCall)
                {
                    PopTempID();
                    ScopeList.pop_back();
                }

                /* ------------------------------- Arithmethic ------------------------------ */
                else if (ParserTempID == TempID::VariableDefition)
                {
                    if (GetTypeOfNode() == NodeType::AssignVariable)
                    {
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        AddToArithmethicNode(AssignVariableNode, value, LineNumber);
                    }
                }
                break;
            }

            /* --------------------------------- L brace -------------------------------- */
            case lt::LBRACE: // * { sign
            {
                /* --------------------------- Defining functions --------------------------- */
                if (ParserTempID == TempID::FunctionScope)
                {
                    PopTempID();

                    // Add it as a scope
                    auto& EdefNode = dynamic_cast<Edef&>(*Scope->back());
                    AddScope(&EdefNode.FunctionBody.ConnectedNodes);
                }

                /* ------------------------------ If Statements ----------------------------- */
                else if (ParserTempID == TempID::IfElseScope)
                {
                    PopTempID();

                    // Add it as a scope
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    AddScope(&IfElseNode.IfElseBody.ConnectedNodes);
                }

                /* ----------------------------- Else Statements ---------------------------- */
                else if (ParserTempID == TempID::ElseDefinition)
                {
                    PopTempID();

                    // Add it as a scope
                    auto& IfNode = dynamic_cast<IfElse&>(*Scope->back());
                    auto& ElseNode = dynamic_cast<IfElse&>(*IfNode.ElseStatement);
                    AddScope(&ElseNode.IfElseBody.ConnectedNodes);
                }

                else 
                {
                    throw error::RendorException((boost::format("Syntax Error: { found outside of function/if/else scope; Line %s") % LineNumber).str());
                }
                break;
            }

            /* --------------------------------- R brace -------------------------------- */
            case lt::RBRACE: // * } sign
            {
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
                    if (GetTypeOfNode() == NodeType::AssignVariable)
                    {
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        if (AssignVariableNode.Value)
                        {
                            if (AssignVariableNode.Value->Type == NodeType::Arithmethic)
                            {
                                auto& ArithNode = static_cast<Arithmethic&>(*AssignVariableNode.Value);
                                ArithNode.Value += value;
                            }
                        }
                        else 
                        {
                            AssignVariableNode.Value = std::make_unique<Int>(value, LineNumber);
                        }
                    }
                }
                
                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    PushToScope(std::make_unique<Int>(value, LineNumber));
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    AddTokenToConditions<IfElse, Int>(IfElseNode, value, LineNumber);
                }
                break;
            }

            /* ---------------------------------- float --------------------------------- */
            case lt::FLOAT:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    if (GetTypeOfNode() == NodeType::AssignVariable)
                    {
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        if (AssignVariableNode.Value)
                        {
                            if (AssignVariableNode.Value->Type == NodeType::Arithmethic)
                            {
                                auto& ArithNode = static_cast<Arithmethic&>(*AssignVariableNode.Value);
                                ArithNode.Value += value;
                            }
                        }
                        else 
                        {
                            AssignVariableNode.Value = std::make_unique<Double>(value, LineNumber);
                        }
                    }
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    PushToScope(std::make_unique<Double>(value, LineNumber));
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    AddTokenToConditions<IfElse, Double>(IfElseNode, value, LineNumber);
                }
                break;
            }

            /* --------------------------------- string --------------------------------- */
            case lt::STRING:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    if (GetTypeOfNode() == NodeType::AssignVariable)
                    {
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        AssignVariableNode.Value = std::make_unique<String>(value, LineNumber);
                    }
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    PushToScope(std::make_unique<String>(value, LineNumber));
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    AddTokenToConditions<IfElse, String>(IfElseNode, value, LineNumber);
                }
                break;
            }
            
            /* ---------------------------------- bool ---------------------------------- */
            case lt::BOOL:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    if (GetTypeOfNode() == NodeType::AssignVariable)
                    {
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        AssignVariableNode.Value = std::make_unique<Bool>(value, LineNumber);
                    }
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    PushToScope(std::make_unique<Bool>(value, LineNumber));
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    AddTokenToConditions<IfElse, Bool>(IfElseNode, value, LineNumber);
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
                    PopTempID();
                    AddTempID(TempID::FunctionDefiniton);
                    if (*ScopeList.back() != Script.Global.ConnectedNodes)
                    {
                        throw error::RendorException("Compiler Error!");
                    }
                }

                else if 
                ((value == "if") ||
                (value == "else"))
                {
                    if (GetTypeOfNode() == NodeType::IfElse)
                    {
                        if (value == "else")
                        {
                            // ! Else statements are not supported
                            break;
                        }
                    }
                    std::unique_ptr<IfElse> IfElseNode = std::make_unique<IfElse>(LineNumber);
                    IfElseNode->Conditions = std::make_unique<Condition>(LineNumber);
                    
                    ReplaceNode(std::move(IfElseNode));
                    PopTempID();
                    AddTempID(TempID::IfStatementDefinition);
                }

                else if (value == "~forward")
                {
                    PopTempID();
                    AddTempID(TempID::FowardDefinition);
                    if (*ScopeList.back() != Script.Global.ConnectedNodes)
                    {
                        throw error::RendorException("Compiler Error!");
                    }
                }

                else 
                {
                    throw error::RendorException("Not supported yet");
                }
                break;
            }

            /* -------------------------------------------------------------------------- */
            /*                              Binary Operators                              */
            /* -------------------------------------------------------------------------- */
            case lt::BIOP:
            {
                if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    if (IfElseNode.Conditions->Operator) 
                    {
                        /*
                        TODO: Implement nested condition nodes for operators "or", "and", etc.
                        */

                        // ! Temporary, not permanent
                        throw error::RendorException((boost::format("Syntax Error: extra Binary Operator found in if statement; Line %s") % LineNumber).str());
                    }
                    IfElseNode.Conditions->Operator = std::make_unique<BiOp>(value, LineNumber);
                }
                
                else if (ParserTempID == TempID::VariableDefition)
                {
                    if (GetTypeOfNode() == NodeType::AssignVariable)
                    {
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        AddToArithmethicNode(AssignVariableNode, value, LineNumber);
                    }
                }
                break;
            }

            /* -------------------------------------------------------------------------- */
            /*                           Anything not supported                           */
            /* -------------------------------------------------------------------------- */
            default:
            {
                throw error::RendorException("Not supported yet");
            }
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
        DeltaInspectAST(Node);
        ByteCode.emplace_back(ByteCodeGen(Node->Type, Node));
    }
    /* ------------------------- ending the global scope ------------------------ */
    ByteCode.emplace_back("END 0"); 
    /* ---------------------------- Optimize bytecode --------------------------- */
    RendorDeltaOptimizer::DeltaOptimizer(ByteCode);
    /* ----------------------------- return bytecode ---------------------------- */
    return ByteCode;
}
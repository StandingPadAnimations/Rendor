#include "RendorCompiler/Parser/Parser.hpp"
#include <memory>
#include <utility>

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
void Parser::ASTGeneration(const std::vector<std::pair<Lex::Token, std::string>>& Tokens)
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
                (ParserTempID != TempID::IfElseScope) &&
                (ParserTempID != TempID::NameSpace))
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
                        throw error::RendorException(fmt::format("Syntax Error: {} found during the definition of {}; Line {}", value, AssignVariableNode.VariableName, LineNumber));
                    }
                }

                else if (ParserTempID == TempID::NameSpace)
                {
                    // Add namespace to body
                    ParentNodes.back()->NameSpace = value;
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

                else if (ParserTempID == TempID::Extern)
                {
                    PopTempID();
                    std::unique_ptr<FowardEdef> ForwardEdefNode = std::make_unique<FowardEdef>(value, LineNumber);
                    ForwardEdefNode->Extern = true;
                    PushToScope(std::move(ForwardEdefNode));
                    AddTempID(TempID::FowardDefinition);
                }

                /* -------------------------- if it is an argument -------------------------- */
                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    PushToScope(std::make_unique<Reference>(value, LineNumber)); // Add argument to Node
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

                else if (ParserTempID == TempID::FowardDefinition)
                {
                    PushToScope(std::make_unique<FowardEdef>(value, LineNumber)); // Add argument to Node
                }
                
                else if (ParserTempID == TempID::FowardArgsDefinition)
                {
                    if (GetTypeOfNode() == NodeType::FowardEdef)
                    {
                        auto& FowardNode = static_cast<FowardEdef&>(*Scope->back());

                        // Check if it's an extern statement
                        if (FowardNode.Extern)
                        {
                            throw error::RendorException(fmt::format("{} is not a typehint; Line {}", value, LineNumber));
                        }

                        // grab the identifier and type
                        auto& [Identifier, Type] = FowardNode.Args.back();

                        // if the identifer is a placeholder
                        if (Identifier == "@placeholder")
                        {
                            Identifier = value;
                            break;
                        }
                        FowardNode.Args.emplace_back(value, NodeType::Any); // Add argument to Node with type Any
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
                    throw error::RendorException(fmt::format("Syntax Error: = found on line {}, expected variable definition", LineNumber));
                }
                break;
            }

            /* ---------------------------------- Comma --------------------------------- */
            case lt::COMMA:
            {
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

                else if (ParserTempID == TempID::NameSpace)
                {
                    PopTempID();
                }

                else 
                {
                    throw error::RendorException(fmt::format("Syntax Error: {{ found outside of function/if/else scope; Line {}", LineNumber));
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

                else if (ParserTempID == TempID::Import)
                {
                    if (GetTypeOfNode() == NodeType::Import)
                    {
                        auto& ImportNode = static_cast<Import&>(*Scope->back());
                        ImportNode.Name = value;
                    }
                    PopTempID();
                }

                else if (ParserTempID == TempID::NameSpace)
                {
                    if (GetTypeOfNode() == NodeType::Body)
                    {
                        auto& BodyNode = static_cast<Body&>(*Scope->back());
                        BodyNode.NameSpace = value;
                    }
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
                        AssignVariableNode.Value = std::make_unique<Bool>(std::string{value}, LineNumber);
                    }
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    PushToScope(std::make_unique<Bool>(std::string{value}, LineNumber));
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    AddTokenToConditions<IfElse, Bool>(IfElseNode, value, LineNumber);
                }
                break;
            }
            
            case lt::TYPE_HINT:
            {
                if (GetTypeOfNode() == NodeType::FowardEdef)
                {
                    auto& FowardEdefNode = dynamic_cast<FowardEdef&>(*Scope->back());

                    // @ is not a valid characther to use in an argument, thus it's safe to use
                    FowardEdefNode.Args.emplace_back("@placeholder", TypeTable.at(value));
                }
                break;
            }

            case lt::ATTRIBUTE:
            {
                PushToScope(std::make_unique<Attribute>(LineNumber));
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

                /* --------------------------------- Imports -------------------------------- */
                else if (value == "cimport")
                {
                    PopTempID();
                    AddTempID(TempID::Import);
                    std::unique_ptr<Import> ImportNode = std::make_unique<Import>(LineNumber);
                    ImportNode->CImport = true; 
                    PushToScope(std::move(ImportNode));
                }
                
                /* --------------------------------- Exports -------------------------------- */
                else if (value == "export")
                {
                    PopTempID();
                    std::unique_ptr<Export> ExportNode = std::make_unique<Export>(LineNumber);
                    AddScope(&ExportNode->ExportBody.ConnectedNodes);
                    PushToScope(std::move(ExportNode));
                }

                /* ------------------------------- Namespaces ------------------------------- */
                else if (value == "namespace")
                {
                    PopTempID();
                    std::unique_ptr<Body> BodyNode = std::make_unique<Body>();
                    AddParentNode(BodyNode.get()); // Add it to parent nodes
                    AddScope(&BodyNode->ConnectedNodes); // Add the body to scopes
                    PushToScope(std::move(BodyNode)); // Add the node to the current scope
                    AddTempID(TempID::NameSpace);
                }

                else if (value == "extern")
                {
                    PopTempID();
                    AddTempID(TempID::Extern);
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
                        throw error::RendorException(fmt::format("Syntax Error: extra Binary Operator found in if statement; Line {}", LineNumber));
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
}
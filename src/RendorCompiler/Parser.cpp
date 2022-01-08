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
static std::string ByteCodeGen(const NodeType& ClassType, const NodeObject& NodeClass, std::vector<std::string>& ByteCode);
static void TypeConstants(const NodeObject& Node, std::vector<std::string>& ByteCode);

/* -------------------------------------------------------------------------- */
/*                               Parser function                              */
/* -------------------------------------------------------------------------- */
std::vector<std::string> Parser(const std::vector<std::pair<Lex::Token, std::string>>& Tokens)
{
    // Node related stuff
    Main Script;
    std::vector<std::vector<std::unique_ptr<Node>>*> ScopeList {&Script.Global.ConnectedNodes}; // Scopes

    // Parser related stuff
    TempID ParserTempID = TempID::None;
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
                    if (Scope->back()->Type == NodeType::AssignVariable)
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
                    Scope->push_back(std::make_unique<Edef>(value));
                }

                /* -------------------------- if it is an argument -------------------------- */
                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.push_back(std::make_unique<Reference>(value)); // Add argument to Node
                }

                /* ------------- if it's a scoped argument(like edef lol(world)) ------------ */
                else if (ParserTempID == TempID::FunctionArgumentsDefinition) // Function arguments 
                {
                    auto& EdefNode = dynamic_cast<Edef&>(*Scope->back());
                    EdefNode.Args.emplace_back(value); // Add argument to Node
                }

                /* ------------------------------ if statements ----------------------------- */
                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    
                    /* ---------------------- If there's nothing in vector ---------------------- */
                    if (IfElseNode.Conditions.size() == 0)
                    {
                        IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                    }
                    else 
                    {
                        /* ------------- If previous condition node is already finished ------------- */
                        if 
                        ((IfElseNode.Conditions.back()->Condition1) &&
                        (IfElseNode.Conditions.back()->Condition2)  &&
                        (IfElseNode.Conditions.back()->Operator))
                        {
                            IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                            IfElseNode.Conditions.back()->Condition1 = std::make_unique<Reference>(value); // Add it as first condition no questions asked
                            break;
                        }
                    }

                    /* ---------------------------- Adding Conditions --------------------------- */
                    if (IfElseNode.Conditions.back()->Operator)
                    {
                        IfElseNode.Conditions.back()->Condition2 = std::make_unique<Reference>(value);
                    }
                    else 
                    {
                        IfElseNode.Conditions.back()->Condition1 = std::make_unique<Reference>(value);
                    }
                }
                
                /* ----------------------------- Add as variable ---------------------------- */
                else 
                {
                    Scope->push_back(std::make_unique<Reference>(value));
                    ParserTempID = TempID::IdentifierDefinition;
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
                    AssignmentNode.Value = std::make_unique<FunctionCall>(value);
                }
                else
                {
                    Scope->push_back(std::make_unique<FunctionCall>(value));
                }
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
                    auto& ReferenceNode = static_cast<Reference&>(*Scope->back());
                    Scope->back() = std::make_unique<AssignVariable>(ReferenceNode.Value); 
                    ParserTempID = TempID::VariableDefition;
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    auto& ConditionNode = static_cast<Condition&>(*IfElseNode.Conditions.back());
                    
                    if(!ConditionNode.Operator)
                    {
                        ConditionNode.Operator = std::make_unique<BiOp>();
                    }
                    
                    (*ConditionNode.Operator).Operator += value;
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
                    if (Scope->back()->Type == NodeType::FunctionCall)
                    {
                        auto& FunctionNode = static_cast<FunctionCall&>(*Scope->back());
                        if (FunctionNode.Args.back()->Type == NodeType::Reference)
                        {
                            auto& ReferenceNode = static_cast<Reference&>(*FunctionNode.Args.back());
                            std::unique_ptr<FunctionCall> FunctionCallNode = std::make_unique<FunctionCall>(ReferenceNode.Value);
                            FunctionNode.Args.back() = std::move(FunctionCallNode);
                        }
                    }
                }

                /* --------------------------- Defining functions --------------------------- */
                else if (ParserTempID == TempID::FunctionDefiniton) // Defining a funcion 
                {
                    ParserTempID = TempID::FunctionArgumentsDefinition;
                }
                
                /* -------------------- Function Calls in the main scope -------------------- */
                else if (ParserTempID == TempID::IdentifierDefinition)
                {
                    /* ---------------- Check if the last node is an assign node ---------------- */
                    if (Scope->back()->Type == NodeType::AssignVariable)
                    {
                        /* ------------------------ Create FunctionCall Node ------------------------ */
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        std::unique_ptr<FunctionCall> FunctionCallNode = std::make_unique<FunctionCall>(AssignVariableNode.VariableName);
                        
                        /* ------------------------------ Replace Node ------------------------------ */
                        Scope->back() = std::move(FunctionCallNode);
                        ParserTempID = TempID::FunctionCall;
                    }
                }

                /* ------------------------- Defining if statements ------------------------- */
                else if (ParserTempID == TempID::IfStatementDefinition)
                {
                    ParserTempID = TempID::ConditionDefinition;
                }

                /* ------------------------------- Arithmethic ------------------------------ */
                else if (ParserTempID == TempID::VariableDefition)
                {
                    if (Scope->back()->Type == NodeType::AssignVariable)
                    {
                        auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                        
                        switch (AssignVariableNode.Value->Type)
                        {
                            case NodeType::Arithmethic:
                            {
                                auto& ArithmethicNode = static_cast<Arithmethic&>(*AssignVariableNode.Value);
                                ArithmethicNode.Value += value;
                                break;
                            }

                            case NodeType::Int:
                            {
                                auto& IntNode = static_cast<Int&>(*AssignVariableNode.Value);
                                std::unique_ptr<Arithmethic> ArithmethicNode = std::make_unique<Arithmethic>(IntNode.Value);
                                ArithmethicNode->Value += value;

                                AssignVariableNode.Value = std::move(ArithmethicNode);
                                break;
                            }

                            case NodeType::Double:
                            {
                                auto& DoubleNode = static_cast<Double&>(*AssignVariableNode.Value);
                                std::unique_ptr<Arithmethic> ArithmethicNode = std::make_unique<Arithmethic>(DoubleNode.Value);
                                ArithmethicNode->Value += value;

                                AssignVariableNode.Value = std::move(ArithmethicNode);
                                break;
                            }

                            case NodeType::Reference:
                            {
                                auto& ReferenceNode = static_cast<Reference&>(*AssignVariableNode.Value);
                                std::unique_ptr<Arithmethic> ArithmethicNode = std::make_unique<Arithmethic>(ReferenceNode.Value);
                                ArithmethicNode->Value += value;

                                AssignVariableNode.Value = std::move(ArithmethicNode);
                                break;
                            }

                            default:
                            {
                                break;
                            }
                        }
                    }
                    break;
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
                /* --------------------------- Defining functions --------------------------- */
                if (ParserTempID == TempID::FunctionScope)
                {
                    ParserTempID = TempID::None;
                    ++ScopeLevel;

                    // Add it as a scope
                    auto& EdefNode = dynamic_cast<Edef&>(*Scope->back());
                    ScopeList.emplace_back(&EdefNode.FunctionBody.ConnectedNodes);
                }

                /* ------------------------------ If Statements ----------------------------- */
                else if (ParserTempID == TempID::IfElseScope)
                {
                    ParserTempID = TempID::None;
                    ++ScopeLevel;

                    // Add it as a scope
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    ScopeList.emplace_back(&IfElseNode.IfElseBody.ConnectedNodes);
                }

                /* ----------------------------- Else Statements ---------------------------- */
                else if (ParserTempID == TempID::ElseDefinition)
                {
                    ParserTempID = TempID::None;
                    ++ScopeLevel;

                    // Add it as a scope
                    auto& IfNode = dynamic_cast<IfElse&>(*Scope->back());
                    auto& ElseNode = dynamic_cast<IfElse&>(*IfNode.ElseStatement);
                    ScopeList.emplace_back(&ElseNode.IfElseBody.ConnectedNodes);
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
                    auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                    AssignVariableNode.Value = std::make_unique<Int>(value);
                }
                
                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.push_back(std::make_unique<Int>(value)); // Add argument to Node
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    
                    /* ---------------------- If there's nothing in vector ---------------------- */
                    if (IfElseNode.Conditions.size() == 0)
                    {
                        IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                    }
                    else 
                    {
                        /* ------------- If previous condition node is already finished ------------- */
                        if 
                        ((IfElseNode.Conditions.back()->Condition1) &&
                        (IfElseNode.Conditions.back()->Condition2)  &&
                        (IfElseNode.Conditions.back()->Operator))
                        {
                            IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                            IfElseNode.Conditions.back()->Condition1 = std::make_unique<Int>(value); // Add it as first condition no questions asked
                            break;
                        }
                    }

                    /* ---------------------------- Adding Conditions --------------------------- */
                    if (IfElseNode.Conditions.back()->Operator)
                    {
                        IfElseNode.Conditions.back()->Condition2 = std::make_unique<Int>(value);
                    }
                    else 
                    {
                        IfElseNode.Conditions.back()->Condition1 = std::make_unique<Int>(value);
                    }
                }
                break;
            }

            /* ---------------------------------- float --------------------------------- */
            case lt::FLOAT:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                    AssignVariableNode.Value = std::make_unique<Double>(value);
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.push_back(std::make_unique<Double>(value)); // Add argument to Node
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    
                    /* ---------------------- If there's nothing in vector ---------------------- */
                    if (IfElseNode.Conditions.size() == 0)
                    {
                        IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                    }
                    else 
                    {
                        /* ------------- If previous condition node is already finished ------------- */
                        if 
                        ((IfElseNode.Conditions.back()->Condition1) &&
                        (IfElseNode.Conditions.back()->Condition2)  &&
                        (IfElseNode.Conditions.back()->Operator))
                        {
                            IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                            IfElseNode.Conditions.back()->Condition1 = std::make_unique<Double>(value); // Add it as first condition no questions asked
                            break;
                        }
                    }

                    /* ---------------------------- Adding Conditions --------------------------- */
                    if (IfElseNode.Conditions.back()->Operator)
                    {
                        IfElseNode.Conditions.back()->Condition2 = std::make_unique<Double>(value);
                    }
                    else 
                    {
                        IfElseNode.Conditions.back()->Condition1 = std::make_unique<Double>(value);
                    }
                }
                break;
            }

            /* --------------------------------- string --------------------------------- */
            case lt::STRING:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                    AssignVariableNode.Value = std::make_unique<String>(value);
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.push_back(std::make_unique<String>(value)); // Add argument to Node
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    
                    /* ---------------------- If there's nothing in vector ---------------------- */
                    if (IfElseNode.Conditions.size() == 0)
                    {
                        IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                    }
                    else 
                    {
                        /* ------------- If previous condition node is already finished ------------- */
                        if 
                        ((IfElseNode.Conditions.back()->Condition1) &&
                        (IfElseNode.Conditions.back()->Condition2)  &&
                        (IfElseNode.Conditions.back()->Operator))
                        {
                            IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                            IfElseNode.Conditions.back()->Condition1 = std::make_unique<String>(value); // Add it as first condition no questions asked
                            break;
                        }
                    }

                    /* ---------------------------- Adding Conditions --------------------------- */
                    if (IfElseNode.Conditions.back()->Operator)
                    {
                        IfElseNode.Conditions.back()->Condition2 = std::make_unique<String>(value);
                    }
                    else 
                    {
                        IfElseNode.Conditions.back()->Condition1 = std::make_unique<String>(value);
                    }
                }
                break;
            }
            
            /* ---------------------------------- bool ---------------------------------- */
            case lt::BOOL:
            {
                if (ParserTempID == TempID::VariableDefition) // variables
                {
                    auto& AssignVariableNode = static_cast<AssignVariable&>(*Scope->back());
                    AssignVariableNode.Value = std::make_unique<Bool>(value);
                }

                else if (ParserTempID == TempID::FunctionCall) // function calls
                {
                    auto& FunctionNode = dynamic_cast<FunctionCall&>(*Scope->back());
                    FunctionNode.Args.push_back(std::make_unique<Bool>(value)); // Add argument to Node
                }

                else if (ParserTempID == TempID::ConditionDefinition)
                {
                    /* -------------------------- Adding Condition Node ------------------------- */
                    auto& IfElseNode = dynamic_cast<IfElse&>(*Scope->back());
                    
                    /* ---------------------- If there's nothing in vector ---------------------- */
                    if (IfElseNode.Conditions.size() == 0)
                    {
                        IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                    }
                    else 
                    {
                        /* ------------- If previous condition node is already finished ------------- */
                        if 
                        ((IfElseNode.Conditions.back()->Condition1) &&
                        (IfElseNode.Conditions.back()->Condition2)  &&
                        (IfElseNode.Conditions.back()->Operator))
                        {
                            IfElseNode.Conditions.push_back(std::make_unique<Condition>()); // Add condition 
                            IfElseNode.Conditions.back()->Condition1 = std::make_unique<Bool>(value); // Add it as first condition no questions asked
                            break;
                        }
                    }

                    /* ---------------------------- Adding Conditions --------------------------- */
                    if (IfElseNode.Conditions.back()->Operator)
                    {
                        IfElseNode.Conditions.back()->Condition2 = std::make_unique<Bool>(value);
                    }
                    else 
                    {
                        IfElseNode.Conditions.back()->Condition1 = std::make_unique<Bool>(value);
                    }
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
                    if (Scope->back()->Type == NodeType::IfElse)
                    {
                        if (value == "else")
                        {
                            auto& IfElseNode = static_cast<IfElse&>(*Scope->back());
                            IfElseNode.ElseStatement = std::make_unique<IfElse>();
                            ScopeList.emplace_back(&IfElseNode.ElseStatement->IfElseBody.ConnectedNodes);
                            ParserTempID = TempID::ElseDefinition;
                            break;
                        }
                    }
                    
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
            /*                              Binary Operators                              */
            /* -------------------------------------------------------------------------- */
            case lt::BIOP:
            {
                // ! Fix this 
                // ! ^^^^^^^^
                // ! ^^^^^^^^
                // ! ^^^^^^^^
                // ! ^^^^^^^^
                // ! ^^^^^^^^
                // ! ^^^^^^^^
                // ! ^^^^^^^^
                // ! ^^^^^^^^
                // ! ^^^^^^^^
                // ! ^^^^^^^^
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
        ByteCode.emplace_back(ByteCodeGen(Node->Type, Node, ByteCode));
    }
    /* ------------------------- ending the global scope ------------------------ */
    ByteCode.emplace_back("END 0"); 
    /* ---------------------------- Optimize bytecode --------------------------- */
    RendorDeltaOptimizer::DeltaOptimizer(ByteCode);
    /* ----------------------------- return bytecode ---------------------------- */
    return ByteCode;
}


/* -------------------------------------------------------------------------- */
/*                      Bytecode Generation loop function                     */
/* -------------------------------------------------------------------------- */

static std::string ByteCodeGen(const NodeType& ClassType, const NodeObject& NodeClass, std::vector<std::string>& ByteCode)
{

    /* -------------------------------------------------------------------------- */
    /*                             assigning variables                            */
    /* -------------------------------------------------------------------------- */
    if (ClassType == NodeType::AssignVariable) 
    {
        auto& AssignmentNode = static_cast<AssignVariable&>(*NodeClass); 
        TypeConstants(AssignmentNode.Value, ByteCode);
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

        for (const auto& Node : EdefNode.FunctionBody.ConnectedNodes) // actual body 
        {
            ByteCode.emplace_back(ByteCodeGen(Node->Type, Node, ByteCode));
        }

        return "FUNCTION END";
    }

    else if (ClassType == NodeType::FunctionCall)
    {
        TypeConstants(NodeClass, ByteCode);
    }

    /* -------------------------------------------------------------------------- */
    /*                         Defining if-else statements                        */
    /* -------------------------------------------------------------------------- */
    else if (ClassType == NodeType::IfElse)
    {
        auto& IfElseNode = static_cast<IfElse&>(*NodeClass); 
        auto& ConditionNode = static_cast<Condition&>(*NodeClass); 
        TypeConstants(ConditionNode.Condition1, ByteCode);
        TypeConstants(ConditionNode.Condition2, ByteCode);

        if (ConditionNode.Operator->Operator == "==")
        {
            ByteCode.emplace_back("OPERATOR EQUAL");
        }
        else if (ConditionNode.Operator->Operator == "!=")
        {
            ByteCode.emplace_back("OPERATOR NOT_EQUAL");
        }
        else if (ConditionNode.Operator->Operator == ">")
        {
            ByteCode.emplace_back("OPERATOR GREATER_THAN");
        }
        else if (ConditionNode.Operator->Operator == "<")
        {
            ByteCode.emplace_back("OPERATOR LESS_THAN");
        }
        else if (ConditionNode.Operator->Operator == ">=")
        {
            ByteCode.emplace_back("OPERATOR GREATER_OR_EQUAL");
        }
        else if (ConditionNode.Operator->Operator == "<=")
        {
            ByteCode.emplace_back("OPERATOR LESS_OR_EQUAL");
        }
        ByteCode.emplace_back("JMP_IF_FALSE ");
        
        /* -------------------- For the JMP_IF_FALSE instruction -------------------- */
        size_t IndexOfJMP = ByteCode.size() - 1;
        
        for (const auto& Node : IfElseNode.IfElseBody.ConnectedNodes) // actual body 
        {
            ByteCode.emplace_back(ByteCodeGen(Node->Type, Node, ByteCode));
        }

        ByteCode[IndexOfJMP] += std::to_string(ByteCode.size() - 1);
        return "ENDIF STATE";
    }

    return "ERROR";
}

static void TypeConstants(const NodeObject& Node, std::vector<std::string>& ByteCode)
{
    switch (Node->Type)
    {
        case NodeType::Int:
        {
            auto& IntNode = static_cast<Int&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST %s") % IntNode.Value).str());
            break;
        }

        case NodeType::Double:
        {
            auto& DoubleNode = static_cast<Double&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST %s") % DoubleNode.Value).str());
            break;
        }

        case NodeType::String:
        {
            auto& StringNode = static_cast<String&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST %s") % StringNode.Value).str());
            break;
        }

        case NodeType::Bool:
        {
            auto& BoolNode = static_cast<Bool&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST %s") % BoolNode.Value).str());
            break;
        }

        case NodeType::Arithmethic:
        {
            auto& ArithNode = static_cast<Arithmethic&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST %s") % OperationToPostfix(ArithNode.Value)).str());
            break;
        }

        case NodeType::FunctionCall:
        {
            auto& FunctionCallNode = static_cast<FunctionCall&>(*Node); 

            ByteCode.emplace_back((boost::format("CALL %s") % FunctionCallNode.Function).str());
            for (const auto& arg : FunctionCallNode.Args) // Arguments 
            {
                ByteCode.emplace_back((boost::format("CALL_ARG %s") % arg).str());
            }
            ByteCode.emplace_back((boost::format("FINALIZE_CALL %s") % FunctionCallNode.Function).str());
        }

        default:
        {
            break;
        }
    }
}
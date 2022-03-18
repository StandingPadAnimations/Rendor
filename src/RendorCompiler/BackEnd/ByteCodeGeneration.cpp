#include "RendorCompiler/Parser/Parser.hpp"

/* -------------------------------------------------------------------------- */
/*                      Bytecode Generation loop function                     */
/* -------------------------------------------------------------------------- */

std::string Parser::ByteCodeGen(const NodeType& ClassType, const NodeObject& NodeClass)
{
    /* -------------------------------------------------------------------------- */
    /*                         Defining if-else statements                        */
    /* -------------------------------------------------------------------------- */
    else if (ClassType == NodeType::IfElse)
    {
        auto& IfElseNode = static_cast<IfElse&>(*NodeClass); 
        auto& ConditionNode = IfElseNode.Conditions; 
        TypeConstants(ConditionNode->Condition1->Type, ConditionNode->Condition1);
        TypeConstants(ConditionNode->Condition2->Type, ConditionNode->Condition2);

        if (ConditionNode->Operator->Operator == "==")
        {
            ByteCode.emplace_back("OPERATOR EQUAL");
        }
        else if (ConditionNode->Operator->Operator == "!=")
        {
            ByteCode.emplace_back("OPERATOR NOT_EQUAL");
        }
        else if (ConditionNode->Operator->Operator == ">")
        {
            ByteCode.emplace_back("OPERATOR GREATER_THAN");
        }
        else if (ConditionNode->Operator->Operator == "<")
        {
            ByteCode.emplace_back("OPERATOR LESS_THAN");
        }
        else if (ConditionNode->Operator->Operator == ">=")
        {
            ByteCode.emplace_back("OPERATOR GREATER_OR_EQUAL");
        }
        else if (ConditionNode->Operator->Operator == "<=")
        {
            ByteCode.emplace_back("OPERATOR LESS_OR_EQUAL");
        }
        ByteCode.emplace_back("JMP ");
        
        /* -------------------- For the JMP_IF_FALSE instruction -------------------- */
        size_t IndexOfJMP = ByteCode.size() - 1;
        size_t JumpAmount = 0;
        
        for (const auto& Node : IfElseNode.IfElseBody.ConnectedNodes) // actual body 
        {
            size_t BeforeSize = ByteCode.size();
            ByteCode.emplace_back(ByteCodeGen(Node->Type, Node));
            size_t AfterSize = ByteCode.size();
            JumpAmount += (AfterSize - BeforeSize);
        }
    
        ByteCode[IndexOfJMP] += std::to_string(JumpAmount);
        return "ENDIF STATE";
    }

    return "ERROR";
}

void Parser::TypeConstants(const NodeType& ClassType, const NodeObject& Node)
{
    switch (ClassType)
    {
        case NodeType::Int:
        {
            auto& IntNode = static_cast<Int&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST INT64, %s") % IntNode.Value).str());
            break;
        }

        case NodeType::Double:
        {
            auto& DoubleNode = static_cast<Double&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST DOUBLE, %s") % DoubleNode.Value).str());
            break;
        }

        case NodeType::String:
        {
            auto& StringNode = static_cast<String&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST STRING, %s") % StringNode.Value).str());
            break;
        }

        case NodeType::Bool:
        {
            auto& BoolNode = static_cast<Bool&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST BOOL, %s") % BoolNode.Value).str());
            break;
        }

        case NodeType::Arithmethic:
        {
            auto& ArithNode = static_cast<Arithmethic&>(*Node); 
            std::string Operation = OperationToPostfix(ArithNode.Value);
            ByteCode.emplace_back((boost::format("CONST ARITHMETHIC, %s") % Operation).str());
            break;
        }

        case NodeType::Reference:
        {
            auto& ReferenceNode = static_cast<Reference&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST REFERENCE, %s") % ReferenceNode.Value).str());
            break;
        }

        case NodeType::FunctionCall:
        {
            auto& FunctionCallNode = static_cast<FunctionCall&>(*Node); 

            for (const auto& Arg : FunctionCallNode.Args) // Arguments 
            {
                TypeConstants(Arg->Type, Arg);
            }
            ByteCode.emplace_back((boost::format("FINALIZE_CALL %s") % FunctionCallNode.Function).str());
        }

        default:
        {
            break;
        }
    }
}
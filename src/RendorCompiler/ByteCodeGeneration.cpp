#include "RendorCompiler/Parser.hpp"

/* -------------------------------------------------------------------------- */
/*                      Bytecode Generation loop function                     */
/* -------------------------------------------------------------------------- */

std::string Parser::ByteCodeGen(const NodeType& ClassType, const NodeObject& NodeClass)
{
    /* -------------------------------------------------------------------------- */
    /*                             assigning variables                            */
    /* -------------------------------------------------------------------------- */
    if (ClassType == NodeType::AssignVariable) 
    {
        auto& AssignmentNode = static_cast<AssignVariable&>(*NodeClass);
        TypeConstants(AssignmentNode.Value->Type, AssignmentNode.Value);
        return (boost::format("ASSIGN %s") % AssignmentNode.VariableName).str();
    }

    /* -------------------------------------------------------------------------- */
    /*                             defining functions                             */
    /* -------------------------------------------------------------------------- */
    else if (ClassType == NodeType::Edef)
    {
        auto& EdefNode = static_cast<Edef&>(*NodeClass); 

        ByteCode.emplace_back((boost::format("DEFINE %s") % EdefNode.Name).str());

        for (std::vector<std::pair<std::string, NodeType>>::reverse_iterator Arg = EdefNode.Args.rbegin(); Arg != EdefNode.Args.rend(); ++Arg) // Arguments 
        {
            ByteCode.emplace_back((boost::format("ARGUMENT %s") % Arg->first).str());
        }

        for (const auto& Node : EdefNode.FunctionBody.ConnectedNodes) // actual body 
        {
            ByteCode.emplace_back(ByteCodeGen(Node->Type, Node));
        }

        return "FUNCTION END";
    }

    else if (ClassType == NodeType::FunctionCall)
    {
        TypeConstants(NodeClass->Type, NodeClass);
        return "";
    }

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
        ByteCode.emplace_back("JMP_IF_FALSE ");
        
        /* -------------------- For the JMP_IF_FALSE instruction -------------------- */
        size_t IndexOfJMP = ByteCode.size() - 1;
        
        for (const auto& Node : IfElseNode.IfElseBody.ConnectedNodes) // actual body 
        {
            ByteCode.emplace_back(ByteCodeGen(Node->Type, Node));
        }
    
        ByteCode[IndexOfJMP] += std::to_string(ByteCode.size() - 1);
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
            ByteCode.emplace_back((boost::format("CONST _S%s") % StringNode.Value).str());
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
            std::string Operation = OperationToPostfix(ArithNode.Value);
            ByteCode.emplace_back((boost::format("CONST &_A%s") % Operation).str());
            break;
        }

        case NodeType::Reference:
        {
            auto& ReferenceNode = static_cast<Reference&>(*Node); 
            ByteCode.emplace_back((boost::format("CONST _&%s") % ReferenceNode.Value).str());
            break;
        }

        case NodeType::FunctionCall:
        {
            auto& FunctionCallNode = static_cast<FunctionCall&>(*Node); 

            ByteCode.emplace_back((boost::format("CALL %s") % FunctionCallNode.Function).str());
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
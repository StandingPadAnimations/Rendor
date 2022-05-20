#ifndef AST_GENERATION_FUNCTIONS_HPP
#define AST_GENERATION_FUNCTIONS_HPP
#include "RendorCompiler/Nodes/Nodes.hpp"
#include "Exceptions.hpp"

template<typename T1, typename T2>
inline void AddTokenToConditions(const T1& Node, const std::string &value, const uint32_t& LineNumber)
{
    if 
    ((Node.Conditions->Condition1) &&
    (Node.Conditions->Condition2)  &&
    (Node.Conditions->Operator))
    {
        throw error::RendorException((boost::format("Syntax Error: extra Constant found in if statement; Line %s") % LineNumber).str());
    }

    /* ---------------------------- Adding Conditions --------------------------- */
    if (Node.Conditions->Operator)
    {
        Node.Conditions->Condition2 = std::make_unique<T2>(value, LineNumber);
    }
    else 
    {
        Node.Conditions->Condition1 = std::make_unique<T2>(value, LineNumber);
    }
}

inline void AddToArithmethicNode(AssignVariable& AssignVariableNode, const std::string &value, const uint32_t& LineNumber)
{
    switch (AssignVariableNode.Value->Type)
    {
        case NodeType::Arithmethic:
        {
            auto& ArithmethicNode = static_cast<Arithmethic&>(*AssignVariableNode.Value);
            ArithmethicNode.Value += value;
            break;
        }

        case NodeType::Int64:
        {
            auto& IntNode = static_cast<Int&>(*AssignVariableNode.Value);
            std::unique_ptr<Arithmethic> ArithmethicNode = std::make_unique<Arithmethic>(IntNode.Value, LineNumber);
            ArithmethicNode->Value += value;

            AssignVariableNode.Value = std::move(ArithmethicNode);
            break;
        }

        case NodeType::Double:
        {
            auto& DoubleNode = static_cast<Double&>(*AssignVariableNode.Value);
            std::unique_ptr<Arithmethic> ArithmethicNode = std::make_unique<Arithmethic>(DoubleNode.Value, LineNumber);
            ArithmethicNode->Value += value;

            AssignVariableNode.Value = std::move(ArithmethicNode);
            break;
        }

        case NodeType::Reference:
        {
            auto& ReferenceNode = static_cast<Reference&>(*AssignVariableNode.Value);
            std::unique_ptr<FunctionCall> FunctionCallNode = std::make_unique<FunctionCall>(ReferenceNode.Value, LineNumber);

            AssignVariableNode.Value = std::move(FunctionCallNode);
            break;
        }

        default:
        {
            break;
        }
    }
}

#endif // AST_GENERATION_FUNCTIONS_HPP
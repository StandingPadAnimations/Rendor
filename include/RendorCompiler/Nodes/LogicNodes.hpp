#ifndef LOGIC_NODES_HPP
#define LOGIC_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct BiOp : Node
{
    std::string Operator = "";
    BiOp(std::string OperatorInput, uint32_t LineNumber) : Node(LineNumber), Operator(OperatorInput)
    {
        Type = NodeType::BiOp;
    }
};

struct Condition : Node
{
    NodeObject Condition1;
    std::unique_ptr<BiOp> Operator;
    NodeObject Condition2;

    Condition(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Condition;
    }
};

#endif // LOGIC_NODES_HPP
#ifndef LOGIC_NODES_HPP
#define LOGIC_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct BiOp : Node
{
    std::string Operator = "";
    explicit BiOp(std::string OperatorInput, uint32_t LineNumber) : Node(LineNumber), Operator(std::move(OperatorInput))
    {
        Type = NodeType::BiOp;
    }
    void CodeGen() override
    {
        NodeByteCodeGen.CreateLogic(Operator);
    }
};

struct Condition : Node
{
    NodeObject Condition1;
    std::unique_ptr<BiOp> Operator;
    NodeObject Condition2;

    explicit Condition(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Condition;
    }
    void CodeGen() override
    {
        Condition1->CodeGen();
        Condition2->CodeGen();
        Operator->CodeGen();
    }
};

#endif // LOGIC_NODES_HPP
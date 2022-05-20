#ifndef INCREMENT_DECREMENT_NODES_HPP
#define INCREMENT_DECREMENT_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct Increment : Node
{
    std::string Args = "";
    explicit Increment(std::string& Args, uint32_t LineNumber) : Node(LineNumber), Args(Args)
    {
        Type = NodeType::Increment;
    }

    void CodeGen() override
    {
        NodeByteCodeGen.CreateIncr(Args);
    }
};

struct Decrement : Node
{
    std::string Args = "";
    explicit Decrement(std::string& Args, uint32_t LineNumber) : Node(LineNumber), Args(Args)
    {
        Type = NodeType::Decrement;
    }

    void CodeGen() override
    {
        NodeByteCodeGen.CreateDecr(Args);
    }
};

#endif // INCREMENT_DECREMENT_NODES_HPP
#ifndef VARIABLE_NODE_HPP
#define VARIABLE_NODE_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct AssignVariable : Node
{
    std::string VariableName = "";
    NodeObject Value;

    AssignVariable() = default;
    explicit AssignVariable(std::string& VariableNameInput, uint32_t LineNumber) : Node(LineNumber), VariableName(VariableNameInput)
    {
        Type = NodeType::AssignVariable;
    }

    void CodeGen() override
    {
        Value->CodeGen();
        NodeByteCodeGen.CreateVariable(VariableName);
    }
};

#endif // VARIABLE_NODE_HPP
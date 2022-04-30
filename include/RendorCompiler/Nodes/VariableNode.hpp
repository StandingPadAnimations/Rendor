#ifndef VARIABLE_NODE_HPP
#define VARIABLE_NODE_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct AssignVariable : Node
{
    std::string VariableName = "";
    NodeObject Value{nullptr};
    bool IsArg = false;

    AssignVariable() = default;

    explicit AssignVariable(std::string VariableNameInput, uint32_t LineNumber) : 
    Node(LineNumber), 
    VariableName(std::move(VariableNameInput))
    {
        Type = NodeType::AssignVariable;
    }

    explicit AssignVariable(std::string VariableNameInput, uint32_t LineNumber, bool IsArg) : 
    Node(LineNumber), 
    VariableName(std::move(VariableNameInput)), 
    IsArg(IsArg)
    {
        Type = NodeType::AssignVariable;
    }

    void CodeGen() override
    {
        if (Value)
        {
            Value->CodeGen();
        }
        NodeByteCodeGen.CreateVariable(VariableName);
    }
};

#endif // VARIABLE_NODE_HPP
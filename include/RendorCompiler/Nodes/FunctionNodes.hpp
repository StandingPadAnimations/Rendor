#ifndef FUNCTION_NODES_HPP
#define FUNCTION_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNodes.hpp"

struct FunctionCall : Node
{
    std::string Function = "";
    std::vector<NodeObject> Args;
    explicit FunctionCall(std::string Function, uint32_t LineNumber) : Node(LineNumber), Function(Function)
    {
        Type = NodeType::FunctionCall;
    }

    void CodeGen()
    {
        for (auto const& Node : Args)
        {
            Node->CodeGen();
        }
        NodeByteCodeGen.CallFunction(Function);
    }
};

struct Edef : Node
{
    std::string Name = "";
    Body FunctionBody;
    std::vector<std::pair<std::string, NodeType>> Args;

    explicit Edef(std::string Name, uint32_t LineNumber) : Node(LineNumber), Name(Name)
    {
        Type = NodeType::Edef;
    }

    void CodeGen()
    {
        NodeByteCodeGen.CreateFunc(Name);
        for (std::vector<std::pair<std::string, NodeType>>::reverse_iterator Arg = Args; Arg != Args; ++Arg) // Arguments 
        {
            NodeByteCodeGen.CreateVariable(Arg->first);
        }
        FunctionBody.CodeGen()
        NodeByteCodeGen.EndFunc();
    }
};

struct FowardEdef : Node
{
    std::string Name = "";
    std::vector<std::pair<std::string, NodeType>> Args;

    explicit FowardEdef(std::string Name, uint32_t LineNumber) : Node(LineNumber), Name(Name)
    {
        Type = NodeType::FowardEdef;
    }
    void CodeGen()
    {
        return;
    }
};

#endif // FUNCTION_NODES_HPP
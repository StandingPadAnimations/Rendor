#ifndef FUNCTION_NODES_HPP
#define FUNCTION_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNodes.hpp"

struct FunctionCall : Node
{
    std::string Function = "";
    std::vector<NodeObject> Args;
    explicit FunctionCall(std::string Function, uint32_t LineNumber) : Node(LineNumber), Function(std::move(Function))
    {
        Type = NodeType::FunctionCall;
    }

    void CodeGen() override
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

    explicit Edef(std::string Name, uint32_t LineNumber) : Node(LineNumber), Name(std::move(Name))
    {
        Type = NodeType::Edef;
    }

    void CodeGen() override
    {
        NodeByteCodeGen.CreateFunc(Name);
        for (size_t Arg = Args.size(); Arg > 0; --Arg) // Arguments 
        {
            auto& [Argument, TypeOfArgument] = Args[Arg];
            NodeByteCodeGen.CreateVariable(Argument);
        }
        FunctionBody.CodeGen();
        NodeByteCodeGen.CreateFuncEnd();
    }
};

struct FowardEdef : Node
{
    std::string Name = "";
    bool Extern = false;
    std::vector<std::pair<std::string, NodeType>> Args;

    explicit FowardEdef(std::string Name, uint32_t LineNumber) : Node(LineNumber), Name(std::move(Name))
    {
        Type = NodeType::FowardEdef;
    }
    void CodeGen() override
    {
        if (Extern)
        {
            // Do something we'll add later
        }
        return;
    }
};

#endif // FUNCTION_NODES_HPP
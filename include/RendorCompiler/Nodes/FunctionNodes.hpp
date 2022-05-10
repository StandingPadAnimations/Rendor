#ifndef FUNCTION_NODES_HPP
#define FUNCTION_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNodes.hpp"
#include <memory>

struct FunctionCall : Node
{
    std::string Function = "";
    ArgBody Args;
    NodeType ReturnValue = NodeType::Null;

    explicit FunctionCall(std::string Function, uint32_t LineNumber) : Node(LineNumber), Function(std::move(Function))
    {
        Type = NodeType::FunctionCall;
    }

    void CodeGen() override
    {
        Args.CodeGen();
        NodeByteCodeGen.CallFunction(Function);
    }
};

struct Edef : Node
{
    std::string Name = "";
    Body FunctionBody;

    // Return type and arguments
    NodeType ReturnType = NodeType::Any;
    std::vector<std::pair<std::string, NodeType>> Args;

    explicit Edef(std::string Name, uint32_t LineNumber) : Node(LineNumber), Name(std::move(Name))
    {
        Type = NodeType::Edef;
    }

    void CodeGen() override
    {
        NodeByteCodeGen.CreateFunc(Name);
        std::reverse(Args.begin(), Args.end()); // Reverse arguments
        for (auto const& [Arg, Type] : Args) // Arguments 
        {
            NodeByteCodeGen.CreateVariable(Arg);
        }
        FunctionBody.CodeGen();
        NodeByteCodeGen.CreateFuncEnd();
    }
};

struct FowardEdef : Node
{
    std::string Name = "";
    std::string MangledName = "";
    NodeType ReturnType = NodeType::Any;

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
            NodeByteCodeGen.RedefineCppName(Name, MangledName);
        }
        return;
    }
};

#endif // FUNCTION_NODES_HPP
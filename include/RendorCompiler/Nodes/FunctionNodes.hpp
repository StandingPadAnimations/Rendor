#ifndef FUNCTION_NODES_HPP
#define FUNCTION_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNodes.hpp"
#include <memory>

struct FunctionCall : Node
{
    std::string Function = "";
    Body Args;
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
    std::vector<std::pair<std::string, NodeType>> Args;

    explicit Edef(std::string Name, uint32_t LineNumber) : Node(LineNumber), Name(std::move(Name))
    {
        Type = NodeType::Edef;
    }

    void CodeGen() override
    {
        NodeByteCodeGen.CreateFunc(Name);

        if (!Args.empty())
        {
            for (size_t Arg = Args.size() - 1; Arg > 0; --Arg) // Arguments 
            {
                NodeByteCodeGen.CreateVariable(Args[Arg].first);
            }
        }
        FunctionBody.CodeGen();
        NodeByteCodeGen.CreateFuncEnd();
    }
};

struct FowardEdef : Node
{
    std::string Name = "";
    std::string MangledName = "";

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
#ifndef RENDOR_FUNCTION_NODE_HPP
#define RENDOR_FUNCTION_NODE_HPP
#include <string>

#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNode.hpp"
#include "RendorCompiler/Nodes/ConstantsNodes.hpp"

#include "RendorCompiler/BackEnd/IR/IRContext.hpp"
namespace nodes
{
    struct FunctionDefinition : Node
    {
        std::string Name{};
        FunctionDefinition() : Node(NodeType::FUNCTION_DEFINITION){}

        Body FunctionBody{};
        std::vector<std::unique_ptr<Constant>> Args;
        IR_Type RetType{};

        void PrintAST() final 
        {
            fmt::print("Edef: {} (", Name);
            if (!Args.empty())
                fmt::print("\n");

            for (const auto& Arg : Args)
            {
                PrintDepth();
                Arg->PrintAST();
            }
            if (!Args.empty())
            {
                PrintDepth(1);
            }
            fmt::print(")\n");
            FunctionBody.PrintAST();
        }
        void CodeGen()
        {
            
        }
    };
}

#endif // RENDOR_FUNCTION_NODE_HPP
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
                for (std::size_t i = 0; i < Depth; ++i)
                    fmt::print("----");
                Arg->PrintAST();
            }
            if (!Args.empty())
            {
                for (std::size_t i = 0; i < Depth+1; ++i)
                    fmt::print("----");
            }
            fmt::print(")\n");
            FunctionBody.PrintAST();
        }
    };

    struct FunctionCall : Node 
    {
        std::string Name{};
        std::vector<std::unique_ptr<Constant>> Args;
        FunctionCall() : Node(NodeType::FUNCTION_CALL){}
        void PrintAST() final 
        {
            fmt::print("Function Call: {} (", Name);
            for (const auto& Arg : Args)
            {
                if (!Args.empty())
                    fmt::print("\n");

                for (std::size_t i = 0; i < Depth+1; ++i)
                    fmt::print("----");
                Arg->PrintAST();
            }
            if (!Args.empty())
            {
                for (std::size_t i = 0; i < Depth; ++i)
                    fmt::print("----");
            }
            fmt::print(")\n");
        }
    };
}

#endif // RENDOR_FUNCTION_NODE_HPP
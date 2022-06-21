#ifndef RENDOR_BODY_NODE_HPP
#define RENDOR_BODY_NODE_HPP
#include <fmt/core.h>
#include <vector>
#include <memory>

#include "RendorCompiler/Nodes/BaseNode.hpp"
namespace nodes 
{
    struct Body : Node 
    {
        std::vector<std::unique_ptr<Node>> Nodes;
        Body() : Node(NodeType::BODY){}
        void PrintAST() final
        {
            ++Depth;
            for (const auto& Node : Nodes)
            {
                PrintDepth();
                Node->PrintAST();
            }
            --Depth;
        }
        void CodeGen() final 
        {
            for (const auto& Node : Nodes)
            {
                Node->CodeGen();
            }
        }
    };

    struct ScriptBody : Node
    {
        std::vector<std::unique_ptr<Node>> Nodes;
        ScriptBody() : Node(NodeType::BODY){}
        void PrintAST() final
        {
            ++Depth;
            for (const auto& Node : Nodes)
            {
                Node->PrintAST();
            }
            --Depth;
        }
        void CodeGen() final 
        {
            for (const auto& Node : Nodes)
            {
                Node->CodeGen();
            }
        }
    };
}

#endif // RENDOR_BODY_NODE_HPP
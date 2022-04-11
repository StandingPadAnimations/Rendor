#ifndef BODY_NODES_HPP
#define BODY_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct Body : Node
{
    std::vector<NodeObject> ConnectedNodes;
    std::string NameSpace = "";
    Body()
    {
        Type = NodeType::Body;
    }
    void CodeGen() override
    {
        for (const auto& Node : ConnectedNodes) // actual body 
        {
            Node->CodeGen();
        }
    }
};

#endif // BODY_NODES_HPP
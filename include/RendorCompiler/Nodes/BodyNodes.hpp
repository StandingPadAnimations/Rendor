#ifndef BODY_NODES_HPP
#define BODY_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct Body : Node
{
    std::vector<NodeObject> ConnectedNodes;
    Body()
    {
        Type = NodeType::Body;
    }
    void CodeGen()
    {
        for (const auto& Node : ConnectedNodes) // actual body 
        {
            Node->CodeGen();
        }
    }
};

#endif // BODY_NODES_HPP
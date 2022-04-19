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
        ConnectedNodes.reserve(100);
    }
    void CodeGen() override
    {
        for (const auto& Node : ConnectedNodes) // actual body 
        {
            Node->CodeGen();
        }
    }

    bool operator!=(const Body& Comp)
    {
        return this->ConnectedNodes != Comp.ConnectedNodes;
    }
};

#endif // BODY_NODES_HPP
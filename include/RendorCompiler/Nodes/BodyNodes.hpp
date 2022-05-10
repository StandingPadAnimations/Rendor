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

    virtual void PushBack(NodeObject Obj)
    {
        ConnectedNodes.push_back(std::move(Obj));
    }

    bool operator!=(const Body& Comp)
    {
        return this->ConnectedNodes != Comp.ConnectedNodes;
    }
};

struct ArgBody : Body
{
    std::vector<NodeType> Types;
    void PushBack(NodeObject Obj) override
    {
        Types.push_back(Obj->Type);
        ConnectedNodes.push_back(std::move(Obj));
    }
};

#endif // BODY_NODES_HPP
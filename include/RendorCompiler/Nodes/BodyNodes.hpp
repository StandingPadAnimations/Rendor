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
};

struct Scope : Node
{
    std::string ScopeName = "";
    Body ScopeBody;
    Scope(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Scope;
    }
};

#endif // BODY_NODES_HPP
#ifndef RENDOR_BODY_NODE_HPP
#define RENDOR_BODY_NODE_HPP
#include <vector>
#include <memory>

#include "RendorCompiler/Nodes/BaseNode.hpp"

namespace nodes 
{
    struct Body : Node 
    {
        std::vector<std::unique_ptr<Node>> Nodes;
        Body() : Node(NodeType::BODY) {}
    };
}

#endif // RENDOR_BODY_NODE_HPP
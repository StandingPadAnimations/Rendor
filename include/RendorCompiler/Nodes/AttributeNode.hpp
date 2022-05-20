#ifndef ATTRIBUTE_NODE_HPP
#define ATTRIBUTE_NODE_HPP
#include "RendorCompiler/Nodes/BaseNode.hpp"
#include <cstdint>

struct Attribute : Node
{
    std::string AttributeDefine = "";

    explicit Attribute(uint32_t LineNumber) : Node(LineNumber) 
    {
        Type = NodeType::Attribute;
    }

    void CodeGen() override
    {
        
    }
};

#endif // ATTRIBUTE_NODE_HPP
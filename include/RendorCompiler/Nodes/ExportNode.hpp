#ifndef EXPORT_NODE_HPP
#define EXPORT_NODE_HPP
#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNodes.hpp"
#include <cstdint>

struct Export : Node
{
    Body ExportBody;
    explicit Export(uint32_t LineNumber) : Node(LineNumber) 
    {
        Type = NodeType::Export;
    }

    void CodeGen() override 
    {
        ExportBody.CodeGen();
    }
};

#endif // EXPORT_NODE_HPP
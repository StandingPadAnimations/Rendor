#ifndef IMPORT_NODES_HPP
#define IMPORT_NODES_HPP
#include "RendorCompiler/Nodes/BaseNode.hpp"

struct Import : Node
{
    explicit Import(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Import;
    }

    std::string Name;
    bool CImport = false;

    void CodeGen() override 
    {
        if (CImport)
        {
            NodeByteCodeGen.CreateCppImport(Name);
        }
    };
};

#endif // IMPORT_NODES_HPP


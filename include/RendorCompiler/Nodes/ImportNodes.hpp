#ifndef RENDOR_IMPORT_NODES_HPP
#define RENDOR_IMPORT_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

namespace nodes
{
    enum class ImportType 
    {
        C_CPP_DLL,
        RENDOR_DYNAMIC,
        RENDOR_STATIC
    };
    struct Import : Node
    {
        std::string Import{};
        ImportType Type{};
        void PrintAST()
        {
            fmt::print("Importing {} of type {}", Import, Type);
        }
    };
}

#endif // RENDOR_IMPORT_NODES_HPP
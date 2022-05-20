#ifndef RENDOR_BYTECODE_CONTEXT_HPP
#define RENDOR_BYTECODE_CONTEXT_HPP
#include <string>
#include <vector>
#include <set>

#include "RendorCompiler/Nodes/NodeType.hpp"
#include "RendorCompiler/BackEnd/ByteCodeType.hpp"

struct RendorContext
{
    std::set<std::string> FunctionTable;
};

#endif // RENDOR_BYTECODE_CONTEXT_HPP
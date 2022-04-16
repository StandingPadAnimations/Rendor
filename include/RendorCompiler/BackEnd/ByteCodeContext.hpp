#ifndef RENDOR_BYTECODE_CONTEXT_HPP
#define RENDOR_BYTECODE_CONTEXT_HPP
#include <string>
#include <vector>
#include <map>

#include "RendorCompiler/Nodes/NodeType.hpp"
#include "RendorCompiler/BackEnd/ByteCodeType.hpp"

struct RendorContext
{
    std::map<std::string, size_t, std::less<>> FunctionTable;
};

#endif // RENDOR_BYTECODE_CONTEXT_HPP
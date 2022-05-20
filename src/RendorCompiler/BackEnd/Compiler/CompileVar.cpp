#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"
#include <bit>
#include <boost/endian/buffers.hpp>

void RendorByteCodeGenerator::CompileVar(const std::string& Name)
{
    WriteRendorOp("AX00");
    WriteString(Name);
}
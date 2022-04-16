#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CreateGlobal()
{
    ByteCode->emplace_back("LOAD 0");
}

void RendorByteCodeGenerator::EndGlobal()
{
    ByteCode->emplace_back("END 0");
}
#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CreateFunc(const std::string& Name)
{
    ByteCode.emplace_back("DEFINE " + Name);
}

void RendorByteCodeGenerator::CreateFuncEnd()
{
    ByteCode.emplace_back("FUNCTION END");
}
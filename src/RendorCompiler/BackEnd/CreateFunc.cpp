#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CreateFunc(const std::string& Name)
{
    ByteCode->emplace_back("DEFINE .F " + Name);
    ++RendorEngineCompiler::MethodCount;
}

void RendorByteCodeGenerator::CreateFuncEnd()
{
    ByteCode->emplace_back("FUNCTION END");
}
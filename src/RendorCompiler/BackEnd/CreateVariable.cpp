#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CreateVariable(const std::string& Name)
{
    ByteCode->emplace_back("ASSIGN" + Name);
}

#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CreateIncr(const std::string& Name)
{
    ByteCode.emplace_back("INCREMENT " + Name);
}
void RendorByteCodeGenerator::CreateDecr(const std::string& Name)
{
    ByteCode.emplace_back("DECREMENT " + Name);
}
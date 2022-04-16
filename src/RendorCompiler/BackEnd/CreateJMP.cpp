#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"


void RendorByteCodeGenerator::CreateJMP(int32_t JMPAmount)
{
    ByteCode->emplace_back("JMP " + std::to_string(JMPAmount));
}

size_t RendorByteCodeGenerator::GetIndex()
{
    return ByteCode->size();
}
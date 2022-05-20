#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CallFunction(const std::string &Name)
{
    ByteCode->emplace_back("FINALIZE_CALL " + Name); 
}
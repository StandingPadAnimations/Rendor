#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CreateCppImport(const std::string& Module)
{
    ByteCode->emplace_back("CIMPORT " + Module);
}
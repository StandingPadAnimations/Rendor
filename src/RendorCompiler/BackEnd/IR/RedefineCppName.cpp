#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::RedefineCppName(const std::string &OriginalName, const std::string &NewName) // NOLINT 
{
    ByteCode->emplace_back(fmt::format("REDEF_CPP {},{}", OriginalName, NewName));
}
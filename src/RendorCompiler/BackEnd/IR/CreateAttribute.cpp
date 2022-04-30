#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CreateAttribute(std::string_view Attr)
{
    if (Attr == "~module_destroy")
    {
        ByteCode->emplace_back("MODULE DESTROY");
    }
}
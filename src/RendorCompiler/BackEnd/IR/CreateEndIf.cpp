#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CreateEndIf()
{
    ByteCode->emplace_back("ENDIF STATE");
}

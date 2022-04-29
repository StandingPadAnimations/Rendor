#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CompileFunc(const std::string& Name)
{
    if (Name == "main")
    {
        WriteRendorOp("DX01");
        const uint32_t STRING_LENGTH = Name.length();
        CrenOutput.write(Name.c_str(), STRING_LENGTH); // name
        return;
    }
    WriteRendorOp("DX00");
    WriteString(Name);
}
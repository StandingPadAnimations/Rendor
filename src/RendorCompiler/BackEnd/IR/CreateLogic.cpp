#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"


void RendorByteCodeGenerator::CreateLogic(const std::string& Operator)
{
    if (Operator == "==")
    {
        ByteCode->emplace_back("OPERATOR EQUAL");
    }
    else if (Operator == "!=")
    {
        ByteCode->emplace_back("OPERATOR NOT_EQUAL");
    }
    else if (Operator == ">")
    {
        ByteCode->emplace_back("OPERATOR GREATER_THAN");
    }
    else if (Operator == "<")
    {
        ByteCode->emplace_back("OPERATOR LESS_THAN");
    }
    else if (Operator == ">=")
    {
        ByteCode->emplace_back("OPERATOR GREATER_OR_EQUAL");
    }
    else if (Operator == "<=")
    {
        ByteCode->emplace_back("OPERATOR LESS_OR_EQUAL");
    }
}

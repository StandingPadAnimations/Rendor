#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"


void RendorByteCodeGenerator::CreateConst(ByteCodeType Type, const std::string& Value)
{
    switch (Type)
    {
        case ByteCodeType::INT32:
        {
            ByteCode->emplace_back("CONST INT32, " + Value);
            break;
        }

        case ByteCodeType::INT64:
        {
            ByteCode->emplace_back("CONST INT64, " + Value);
            break;
        }

        case ByteCodeType::DOUBLE:
        {
            ByteCode->emplace_back("CONST DOUBLE, " + Value);
            break;
        }

        case ByteCodeType::STRING:
        {
            ByteCode->emplace_back("CONST STRING, " + Value);
            break;
        }

        case ByteCodeType::BOOL:
        {
            ByteCode->emplace_back("CONST BOOL, " + Value);
            break;
        }

        case ByteCodeType::ARITH:
        {
            std::string Operation = OperationToPostfix(Value);
            ByteCode->emplace_back("CONST ARITHMETHIC, " + Operation);
            break;
        }

        case ByteCodeType::REFERENCE:
        {
            ByteCode->emplace_back("CONST REFERENCE, " + Value);
            break;
        }

        default:
        {
            break;
        }
    }
}
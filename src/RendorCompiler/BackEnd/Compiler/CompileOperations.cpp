#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

void RendorByteCodeGenerator::CompileCompare(std::string_view Op)
{
    switch (OperatorMapping.at(Op))
    {
        case Operator::EQUAL:
        {
            WriteRendorOp("LX00");
            break;
        }
        case Operator::NOT_EQUAL:
        {
            WriteRendorOp("LX01");
            break;
        }
        case Operator::GREATER_THAN:
        {
            WriteRendorOp("LX02");
            break;
        }
        case Operator::LESS_THAN:
        {
            WriteRendorOp("LX03");
            break;
        }
        case Operator::GREATER_OR_EQUAL:
        {
            WriteRendorOp("LX04");
            break;
        }
        case Operator::LESS_OR_EQUAL:
        {
            WriteRendorOp("LX05");
            break;
        }
        default:
        {
            break;
        }
    }
}

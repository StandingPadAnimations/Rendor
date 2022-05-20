#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"
#include <boost/endian/buffers.hpp>

void RendorByteCodeGenerator::CompileInt64(boost::endian::little_int64_buf_t Int64Value)
{
    WriteRendorOp("CX00");
    CrenOutput.write((char*)&Int64Value, sizeof(Int64Value));
}

void RendorByteCodeGenerator::CompileDouble(boost::endian::little_float64_buf_t DoubleValue)
{
    WriteRendorOp("CX01");
    CrenOutput.write((char*)&DoubleValue, sizeof(DoubleValue));
}

void RendorByteCodeGenerator::CompileString(const std::string& StringValue)
{
    WriteRendorOp("CX02");
    WriteString(StringValue);
}

void RendorByteCodeGenerator::CompileBool(boost::endian::little_int32_buf_at BoolValue)
{

    WriteRendorOp("CX03");
    CrenOutput.write((char*)&BoolValue, sizeof(BoolValue));
}

void RendorByteCodeGenerator::CompileReference(const std::string& Reference)
{
    WriteRendorOp("CX04");
    WriteString(Reference);
}
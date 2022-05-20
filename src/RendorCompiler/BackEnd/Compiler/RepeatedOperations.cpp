#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"
#include <cstdint>

void RendorByteCodeGenerator::WriteRendorOp(const std::string& Operation)
{
    const uint32_t OP_LENGTH = Operation.size();
    CrenOutput.write(Operation.c_str(), OP_LENGTH);
}

void RendorByteCodeGenerator::WriteString(const std::string& String)
{
    const uint32_t STRING_LENGTH = String.length();
    const boost::endian::little_uint32_buf_t STRING_LENGTH_LITTLE = boost::endian::little_uint32_buf_t{STRING_LENGTH};
    CrenOutput.write(std::bit_cast<char*>(&STRING_LENGTH_LITTLE), sizeof(STRING_LENGTH_LITTLE)); // size of the name
    CrenOutput.write(String.c_str(), STRING_LENGTH); // name
}
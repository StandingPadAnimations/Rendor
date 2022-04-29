#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"
#include <boost/endian/buffers.hpp>

struct RendorHeader
{
    boost::endian::little_uint32_buf_t MajorVersion;
    boost::endian::little_uint32_buf_t MinorVersion;
    boost::endian::little_uint32_buf_t MethodCount;

    RendorHeader() :
    MajorVersion(boost::endian::little_uint32_buf_t{RendorEngineCompiler::MajorVersion}),
    MinorVersion(boost::endian::little_uint32_buf_t{RendorEngineCompiler::MinorVersion}),
    MethodCount(boost::endian::little_uint32_buf_t{RendorEngineCompiler::MethodCount}){}
};


void RendorByteCodeGenerator::CompileHeader()
{
    RendorHeader header;
    constexpr const std::string_view MAGICNUMBER = "CHAI";
    constexpr const uint32_t MAGICNUMBER_LENGTH = MAGICNUMBER.length();
    CrenOutput.write(MAGICNUMBER.data(), MAGICNUMBER_LENGTH);

    // Write the header
    CrenOutput.write(std::bit_cast<char*>(&header.MajorVersion), sizeof(header.MajorVersion));
    CrenOutput.write(std::bit_cast<char*>(&header.MinorVersion), sizeof(header.MinorVersion));
    CrenOutput.write(std::bit_cast<char*>(&header.MethodCount),  sizeof(header.MethodCount));
}
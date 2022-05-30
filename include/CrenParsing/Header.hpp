#ifndef RENDOR_HEADER_STRUCT
#define RENDOR_HEADER_STRUCT
#include <cstdint>
#include <string>

struct CrenHeader
{
    std::uint8_t magic_number = 199;
    std::uint8_t major_version = 1;
    std::uint8_t minor_version = 0;
    std::uint32_t function_count = 0;
    std::string identifier = "CHAI";
};

struct SectionHeader 
{
    std::uint8_t type;
};

struct FunctionHeader
{
    std::uint32_t FunctionRegister = 0;
    std::uint32_t ChunkCount = 0;
};
#endif // RENDOR_HEADER_STRUCT
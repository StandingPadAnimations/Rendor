#ifndef RENDOR_HEADER_STRUCT
#define RENDOR_HEADER_STRUCT
#include <cstdint>
#include <string>

struct CrenHeader
{
    std::uint8_t magic_number = 199;
    std::uint8_t major_version = 1;
    std::uint8_t minor_version = 0;
    std::uint32_t main_pos;
    std::uint32_t export_pos;
    std::uint32_t GFT_pos;
    std::uint32_t GVT_pos;
    std::uint32_t pool_pos;
    std::uint32_t str_pool;
    std::uint32_t entry;
    std::string identifier = "CHAI";
};

struct SectionHeader 
{
    std::uint8_t type;
};
#endif // RENDOR_HEADER_STRUCT
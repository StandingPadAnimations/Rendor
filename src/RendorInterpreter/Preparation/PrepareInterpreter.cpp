#include "RendorInterpreter/Interpreter.hpp"
#include <cstring>
#include <type_traits>

bool Interpreter::PrepareInterpreter()
{
    File->seek_absolute(0);
    const auto read_string = [&](std::string& a_dst, std::size_t a_len) {
        a_dst.resize(a_len);
        File->read_bytes(std::as_writable_bytes(std::span{a_dst.data(), a_dst.size()}));
    };
    
    /* --------------------------------- Header --------------------------------- */
    std::tie(header.magic_number,
                header.major_version,
                header.minor_version,
                header.main_pos,
                header.export_pos,
                header.GFT_pos,
                header.GVT_pos,
                header.pool_pos,
                header.str_pool,
                header.entry) = File->read<
                                        std::uint8_t,
                                        std::uint8_t,
                                        std::uint8_t,
                                        std::uint32_t,
                                        std::uint32_t,
                                        std::uint32_t,
                                        std::uint32_t,
                                        std::uint32_t,
                                        std::uint32_t,
                                        std::uint32_t
                                        >(std::endian::little);
    read_string(header.identifier, 5);

    if (header.magic_number != 199)
    {
        return false;
    }
    if (header.identifier == "CHAI")
    {
        return false;
    }

    if (header.main_pos == 0)
    {
        return false;
    }
    File->seek_absolute(0);

    /* ----------------------- GFT (Global Function Table) ---------------------- */
    std::uint32_t Functions;
    File->seek_relative(header.GFT_pos);
    (*File) >> Functions;

    for (std::size_t Func = 0; Func < Functions; ++Func)
    {
        std::string Name;
        std::size_t Size;
        (*File) >> Size;
        read_string(Name, Size);
    }
    return true;
}
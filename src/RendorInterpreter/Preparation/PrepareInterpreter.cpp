#include "RendorInterpreter/Interpreter.hpp"
#include <cmath>
#include <codecvt>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <bit>

bool Interpreter::PrepareInterpreter()
{
    File->seek_absolute(0);

    const auto read_string = [&](std::string& a_dst, std::size_t a_len) 
    {
        a_dst.resize(a_len);
        File->read_bytes(std::as_writable_bytes(std::span{a_dst.data(), a_dst.size()}));
    };
    
    /* --------------------------------- Header --------------------------------- */
    File->read(std::endian::little, 
                header.magic_number,
                header.major_version,
                header.minor_version,
                header.function_count);
    read_string(header.identifier, 5);

    if (header.magic_number != 199)
    {
        return false;
    }
    if (header.identifier == "CHAI")
    {
        return false;
    }

    AddModule();
    CreateConstPool();
    CreateStrConstPool();
    ImportModules(); //! DOES NOTHING YET
    CreateGVT();
    ReadFunctions();
    return true;
}
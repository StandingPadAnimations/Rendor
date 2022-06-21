#include "RendorInterpreter/Interpreter.hpp"
#include <cmath>
#include <codecvt>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <bit>

bool Interpreter::PrepareInterpreter()
{
    if (!File->is_open())
    {
        Stacktrace.emplace_back("File is not opened/does not exist!");
        return false;
    }

    const auto read_string = [&](std::string& a_dst, std::size_t a_len) 
    {
        a_dst.resize(a_len);
        File->read_bytes(std::as_writable_bytes(std::span{a_dst.data(), a_dst.size()}));
    };
    
    /* --------------------------------- Header --------------------------------- */
    File->seek_absolute(0);
    File->read(std::endian::little, 
                header.magic_number,
                header.major_version,
                header.minor_version,
                header.op_count);
    read_string(header.identifier, 5);

    if (header.magic_number != 199)
    {
        Stacktrace.emplace_back("Invalid file! Wrong Magic Number!");
        return false;
    }
    if (header.identifier == "CHAI")
    {
        Stacktrace.emplace_back("Invalid file! Wrong Identifier!");
        return false;
    }

    AddModule();
    CreateConstPool();
    CreateStrConstPool();
    ImportModules(); // ! DOES NOTHING YET
    ReadFunctions();
    return true;
}
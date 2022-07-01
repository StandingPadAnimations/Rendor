#include "RendorInterpreter/Interpreter.hpp"
#include <cmath>
#include <codecvt>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <bit>

bool Interpreter::PrepareInterpreter()
{
    if (!m_File->is_open())
    {
        m_Stacktrace.emplace_back("File is not opened/does not exist!");
        return false;
    }

    const auto read_string = [&](std::string& a_dst, std::size_t a_len) 
    {
        a_dst.resize(a_len);
        m_File->read_bytes(std::as_writable_bytes(std::span{a_dst.data(), a_dst.size()}));
    };
    
    /* --------------------------------- Header --------------------------------- */
    m_File->seek_absolute(0);
    m_File->read(std::endian::little, 
                m_Header.magic_number,
                m_Header.major_version,
                m_Header.minor_version,
                m_Header.op_count,
                m_Header.pool_count);
    read_string(m_Header.identifier, 5);

    if (m_Header.magic_number != 199)
    {
        m_Stacktrace.emplace_back("Invalid file! Wrong Magic Number!");
        return false;
    }
    if (m_Header.identifier == "CHAI")
    {
        m_Stacktrace.emplace_back("Invalid file! Wrong Identifier!");
        return false;
    }

    AddModule();
    CreateConstPool();
    CreateStrConstPool();
    ImportModules(); // ! DOES NOTHING YET
    ReadFunctions();
    return true;
}
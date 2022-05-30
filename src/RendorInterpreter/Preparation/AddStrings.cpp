#include "RendorInterpreter/Interpreter.hpp"

void Interpreter::CreateStrConstPool()
{
    const auto read_string = [&](std::string& a_dst, std::size_t a_len) 
    {
        a_dst.resize(a_len);
        File->read_bytes(std::as_writable_bytes(std::span{a_dst.data(), a_dst.size()}));
    };

    SectionHeader header;
    File->read(header.type);

    if (header.type != 2)
    {
        throw error::RendorException("Invalid binary format!");
    }

    ConstPool* Pool = &CurrentModule->Pool;
    std::uint32_t StringAmount = 0;
    (*File) >> StringAmount;

    for (std::size_t string = 0; string < StringAmount; ++string)
    {
        std::uint16_t index = 0;
        std::uint32_t size = 0;
        std::string String;

        (*File) >> index;
        (*File) >> size;
        read_string(String, size);
        Pool->Pool[index] = Constant{String};
    }
}
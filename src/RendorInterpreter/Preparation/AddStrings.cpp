#include "RendorInterpreter/Interpreter.hpp"

void Interpreter::CreateStrConstPool()
{
    const auto read_string = [&](std::string& a_dst, std::size_t a_len) 
    {
        a_dst.resize(a_len);
        m_File->read_bytes(std::as_writable_bytes(std::span{a_dst.data(), a_dst.size()}));
    };

    SectionHeader Secheader;
    m_File->read(Secheader.type);

    if (Secheader.type != 2)
    {
        throw error::RendorException("Invalid binary format!");
    }

    ConstPool* Pool = &m_CurrentModule->Pool;
    const std::uint32_t StringAmount = [&]()
                                        {
                                            auto [Ret] = m_File->read<std::uint32_t>(std::endian::little);
                                            return Ret;
                                        }();

    for (std::size_t string = 0; string < StringAmount; ++string)
    {
        const std::uint16_t index = [&]()
                                    {
                                        auto [Ret] = m_File->read<std::uint16_t>(std::endian::little);
                                        return Ret;
                                    }();
        const std::uint32_t size = [&]()
                                    {
                                        auto [Ret] = m_File->read<std::uint32_t>(std::endian::little);
                                        return Ret;
                                    }();
        std::string String; read_string(String, size);
        
        Pool->Pool[index] = Constant{String, ConstType::CONST_STR};
    }
}
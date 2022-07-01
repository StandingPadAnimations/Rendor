#include <atomic>
#include <cstdint>
#include <type_traits>

#include "RendorInterpreter/Interpreter.hpp"
#include "CrenParsing/Types.hpp"

void Interpreter::CreateConstPool()
{
    SectionHeader Secheader;
    m_File->read(Secheader.type);
    
    if (Secheader.type != 1)
    {
        throw error::RendorException("Unsupported binary format!");
    }

    m_CurrentModule->Pool.Pool.resize(m_Header.pool_count); // resize pool
    ConstPool* Pool = &m_CurrentModule->Pool;
    
    for (std::size_t Const = 0; Const < m_Header.pool_count; ++Const)
    {
        const std::uint8_t bType = [&]()
                                    {
                                        auto [Ret] = m_File->read<std::uint8_t>(std::endian::little);
                                        return Ret;
                                    }();

        switch (static_cast<Type>(bType))
        {
            case Type::INT:
            {
                RendorInt Value = 0;
                m_File->read(std::endian::little, Value);
                Pool->insert(Constant{Value, ConstType::CONST_NUM});
                break;
            }
            case Type::DOUBLE:
            {
                // ? We have to cast because binary_io doesn't support floating point numbers directly
                std::uint64_t int_Val;
                m_File->read(std::endian::little, int_Val);

                // cast to double
                double Value = std::bit_cast<double>(int_Val);
                Pool->insert(Constant{Value, ConstType::CONST_NUM});
                break;
            }
            case Type::BOOl:
            {
                // ? We have to cast because binary_io doesn't support booleans directly
                std::uint8_t int_Val = 0;
                m_File->read(std::endian::little, int_Val);

                // cast to boolean
                bool Value = std::bit_cast<bool>(int_Val);
                Pool->insert(Constant{RendorConst{Value}, ConstType::CONST_NUM});
                break;
            }
            case Type::VOID:
            {
                Pool->insert(Constant{RendorConst{false}, ConstType::NONE});
                break;
            }
            default:
            {
                throw error::RendorException("Invalid binary format!");
            }
        }
    }
}
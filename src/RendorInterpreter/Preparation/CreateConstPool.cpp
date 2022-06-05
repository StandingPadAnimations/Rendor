#include "RendorInterpreter/Interpreter.hpp"
#include "CrenParsing/Types.hpp"
#include <atomic>
#include <cstdint>
#include <type_traits>

void Interpreter::CreateConstPool()
{
    SectionHeader header;
    File->read(header.type);
    
    if (header.type != 1)
    {
        throw error::RendorException("Unsupported binary format!");
    }

    CurrentModule->Pool = ConstPool();
    ConstPool* Pool = &CurrentModule->Pool;
    std::uint32_t ConstAmount = 0;
    (*File) >> ConstAmount;
    
    for (std::size_t Const = 0; Const < ConstAmount; ++Const)
    {
        std::uint8_t bType;
        (*File) >> bType;

        switch (static_cast<Type>(bType))
        {
            case Type::INT:
            {
                std::int64_t Value = 0;
                File->read(std::endian::little, Value);
                Pool->insert(Constant{Value, ConstType::CONST_NUM});
                break;
            }
            case Type::DOUBLE:
            {
                // ? We have to cast because binary_io doesn't support floating point numbers directly
                std::uint64_t int_Val;
                File->read(std::endian::little, int_Val);

                // cast to double
                double Value = std::bit_cast<double>(int_Val);
                Pool->insert(Constant{Value, ConstType::CONST_NUM});
                break;
            }
            case Type::BOOl:
            {
                // ? We have to cast because binary_io doesn't support booleans directly
                std::uint8_t int_Val = 0;
                File->read(std::endian::little, int_Val);

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
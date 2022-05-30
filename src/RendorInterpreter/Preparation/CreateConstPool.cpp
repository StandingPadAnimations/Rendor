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
                (*File) >> Value;
                Pool->insert(Constant{Value});
                break;
            }
            case Type::DOUBLE:
            {
                double Value = 0.0;
                File->read(std::endian::little, Value);
                Pool->insert(Constant{Value});
                break;
            }
            case Type::BOOl:
            {
                std::uint8_t Value = 0;
                (*File) >> Value;
                Value == true ? Pool->insert(Constant{true}) : Pool->insert(Constant{false});
                break;
            }
            case Type::VOID:
            {
                Pool->insert(Constant{});
                break;
            }
            default:
            {
                throw error::RendorException("Invalid binary format!");
            }
        }
    }
}
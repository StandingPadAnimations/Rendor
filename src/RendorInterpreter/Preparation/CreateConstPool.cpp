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
                Pool->insert(Constant{Value, ConstType::CONST_NUM});
                break;
            }
            case Type::DOUBLE: // ! WILL NOT WORK FOR NOW
            {
                // double Value = 0.0;
                // File->read(std::endian::little, Value);
                // Pool->insert(Constant{Value, ConstType::CONST_NUM});
                break;
            }
            case Type::BOOl:
            {
                std::uint8_t Value = 0;
                (*File) >> Value;
                Value == true ? Pool->insert(Constant{RendorConst{false}, ConstType::CONST_BOOL}) : Pool->insert(Constant{RendorConst{false}, ConstType::CONST_BOOL});
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
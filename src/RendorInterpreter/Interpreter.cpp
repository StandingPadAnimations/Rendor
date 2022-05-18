#include <bit>

#include "RendorInterpreter/Interpreter.hpp"

#include <boost/endian.hpp>
#include <boost/endian/arithmetic.hpp>
#include <boost/range/detail/common.hpp>
#include <type_traits>

namespace be = boost::endian;

template <typename T>
void move_ahead(std::uint32_t& size, const char*& data)
{
    data += sizeof(T);
    size -= sizeof(T);
}

void copy_reg(StackFrame* Frame, Constant* Reg, std::uint32_t& size, const char*& data)
{
    move_ahead<be::little_uint32_t>(size, data);
    be::little_uint32_t value_reg_index = *std::bit_cast<const be::little_uint32_t*>(data);
    Constant* ValueRegister = (*Frame)[value_reg_index]; // register itself
    *Reg = ValueRegister->Const;
}

void Interpreter::ExecuteByteCode(const boost::iostreams::mapped_file &File)
{
    std::uint32_t size = File.size();
    const char* data = File.data();

    ByteCodeEnum ByteCodeOp;
    while (size > 0)
    {
        be::little_int16_t Operation = *std::bit_cast<const be::little_int16_t*>(data);
        ByteCodeOp = static_cast<ByteCodeEnum>(Operation.value());

        switch (ByteCodeOp)
        {
            /* ---------------------------------- Ints ---------------------------------- */
            case ByteCodeEnum::MOVE_I:
            {
                /* ------------------------------ Get register ------------------------------ */
                move_ahead<be::little_uint32_t>(size, data);
                be::little_uint32_t reg_index = *std::bit_cast<const be::little_uint32_t*>(data);
                Constant* Register = (*CurrentStackFrame)[reg_index]; // register itself

                /* --------------------- is the value a register or not? -------------------- */
                move_ahead<be::big_uint8_t>(size, data);
                be::big_uint8_t is_reg = *std::bit_cast<const be::big_uint8_t*>(data);

                /* -------------------------------- add value ------------------------------- */
                if (is_reg)
                {
                    copy_reg(CurrentStackFrame, Register, size, data);
                    break;
                }
                move_ahead<be::little_int64_t>(size, data);
                be::little_int64_t Int64 = *std::bit_cast<const be::little_int64_t*>(data); // get the value
                *Register = Int64.value(); // assign value
                break;
            }

            default:
            {
                throw error::RendorException("Not a bytecode operation!");
            }
        }
    }
}
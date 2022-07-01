#include "RendorInterpreter/Interpreter.hpp"


template<typename ...Base>
struct FunctionVisit : Base...
{
    using Base::operator()...;
};

template<typename ...T> FunctionVisit(T...) -> FunctionVisit<T...>;

void Interpreter::ReadFunctions()
{
    for (std::size_t Func = 0; Func < m_Header.op_count; ++Func)
    {
        Operation FullOp;
        FullOp.Operation = [&]()
                        {
                            auto [Ret] = m_File->read<std::uint8_t>(std::endian::little);
                            return static_cast<ByteCodeEnum>(Ret);
                        }();

        /* ---------------------------- Set up operation ---------------------------- */
        switch (FullOp.Operation)
        {
            case ByteCodeEnum::alloc:
            FALLTHROUGH;
            case ByteCodeEnum::free:
            FALLTHROUGH;
            case ByteCodeEnum::mod_push:
            FALLTHROUGH;
            case ByteCodeEnum::jmp:
            {
                m_File->read(
                    FullOp.Reg_64,
                    FullOp.Reg_64_type_8
                );
                break;
            }
            default:
            {
                m_File->read(
                    FullOp.Reg1_16,
                    FullOp.Reg2_16,
                    FullOp.Reg3_16,
                    FullOp.Reg1_type_8,
                    FullOp.Reg2_type_8,
                    FullOp.Reg3_type_8
                );
                break;
            }
        }
        m_CurrentModule->Program.push_back(std::move(FullOp));
    }
}
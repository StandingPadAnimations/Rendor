#include "RendorInterpreter/Interpreter.hpp"


template<typename ...Base>
struct FunctionVisit : Base...
{
    using Base::operator()...;
};

template<typename ...T> FunctionVisit(T...) -> FunctionVisit<T...>;

void Interpreter::ReadFunctions()
{
    for (std::size_t Func = 0; Func < header.op_count; ++Func)
    {
        Operation FullOp;
        std::uint8_t Op_asInt = 0;

        /* ------------------------------ Get Operation ----------------------------- */
        (*File) >> Op_asInt;
        FullOp.Operation = static_cast<ByteCodeEnum>(Op_asInt);

        /* ---------------------------- Set up operation ---------------------------- */
        switch (FullOp.Operation)
        {
            case ByteCodeEnum::jmp:
            {
                File->read(
                    FullOp.Reg_64,
                    FullOp.Reg_64_type_8
                );
                break;
            }
            default:
            {
                File->read(
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
        CurrentModule->Program.push_back(std::move(FullOp));
    }
}
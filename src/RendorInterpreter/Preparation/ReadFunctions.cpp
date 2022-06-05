#include "RendorInterpreter/Interpreter.hpp"


template<typename ...Base>
struct FunctionVisit : Base...
{
    using Base::operator()...;
};

template<typename ...T> FunctionVisit(T...) -> FunctionVisit<T...>;

void Interpreter::ReadFunctions()
{
    for (std::size_t Func = 0; Func < header.function_count; ++Func)
    {
        FunctionHeader HeaderForFunction;
        std::vector<Operation>* Operations;
        const auto visitor = FunctionVisit 
        {
            [&](Function  Func) -> void {Operations = &Func.FunctionOperations;},
            [&](Function* Func) -> void {Operations = &Func->FunctionOperations;},
        };


        File->read(
            HeaderForFunction.FunctionRegister,
            HeaderForFunction.ChunkCount
        );
        if (HeaderForFunction.FunctionRegister == 0)
        {
            CurrentModule->Main = Function{};
            Operations = &CurrentModule->Main.FunctionOperations;
        } 
        else 
        {
            CurrentModule->Functions.push_back(Function{});
            std::visit(visitor, CurrentModule->Functions.back());
        }

        for (std::size_t chunk = 0; chunk < HeaderForFunction.ChunkCount; ++chunk)
        {
            Operation FullOp;
            std::uint8_t Op_asInt = 0;

            /* ------------------------------ Get Operation ----------------------------- */
            (*File) >> Op_asInt;
            FullOp.Operation = static_cast<ByteCodeEnum>(Op_asInt);

            /* ---------------------------- Set up operation ---------------------------- */
            switch (FullOp.Operation)
            {
                case ByteCodeEnum::call:
                {
                    File->read(
                        FullOp.RetReg,
                        FullOp.FunctionReg,
                        FullOp.Ret_or_not,
                        FullOp.Ret_type
                    );
                    break;
                }
                case ByteCodeEnum::jmp:
                {
                    File->read(
                        FullOp.JMP_amount,
                        FullOp.JMP_amount
                    );
                    break;
                }
                default:
                {
                    File->read(
                        FullOp.Reg1,
                        FullOp.Reg2,
                        FullOp.Reg3,
                        FullOp.Reg1_type,
                        FullOp.Reg2_type,
                        FullOp.Reg3_type
                    );
                    break;
                }
            }
            Operations->push_back(std::move(FullOp));
        }
    }
}
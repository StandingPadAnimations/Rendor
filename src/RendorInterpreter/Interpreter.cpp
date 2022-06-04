#include "RendorInterpreter/Interpreter.hpp"
#include <cstddef>
#include <variant>

void Interpreter::ExecuteByteCode(const Function& Func)
{
    for (std::size_t ip = 0; ip < Func.FunctionOperations.size(); ++ip)
    {
        const Operation* Opera = &Func.FunctionOperations[ip];
        Registers* DstRegisters = nullptr;
        Registers* Op1Registers = nullptr;
        Registers* Op2Registers = nullptr;
        switch (Opera->Operation)
        {
            case ByteCodeEnum::alloc:
            {
                ++sp_int;
                Stack[sp_int] = StackFrame{Opera->Reg1};
                sp = &Stack[sp_int];
                break;
            }
            case ByteCodeEnum::free:
            {
                --sp_int; // We can't do much ¯\_(ツ)_/¯
                sp = &Stack[sp_int];
                break;
            }
            case ByteCodeEnum::call: // ! Fix this
            {
                auto FuncReg = CurrentModule->Functions[Opera->FunctionReg];
                switch (FuncReg.index())
                {
                    case 0:
                    {
                        auto Func = std::get_if<Function>(&FuncReg);
                        ExecuteByteCode(*Func);
                        break;
                    }
                    case 1:
                    {
                        auto Func = std::get_if<Function*>(&FuncReg);
                        ExecuteByteCode(**Func);
                        break;
                    }
                }
                break;
            }
            case ByteCodeEnum::mov:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type);
                SetRegisters(&Op1Registers, Opera->Reg2_type);

                // Move the value
                (*DstRegisters)[Opera->Reg1] = std::move((*Op1Registers)[Opera->Reg2]);
                break;
            }
            case ByteCodeEnum::mov_n:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type); (*DstRegisters)[Opera->Reg1] = Constant{RendorConst{false}, ConstType::NONE};
                break;
            }
            case ByteCodeEnum::cpy:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type);
                SetRegisters(&Op1Registers, Opera->Reg2_type);

                // Copy the value
                (*DstRegisters)[Opera->Reg1] = (*Op1Registers)[Opera->Reg2].clone();
                break;
            }
            case ByteCodeEnum::ref:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type);
                SetRegisters(&Op1Registers, Opera->Reg2_type);

                // Get value to reference
                auto& Val = (*Op1Registers)[Opera->Reg2];
                (*DstRegisters)[Opera->Reg1].Ref = &Val;
                break;
            }
            case ByteCodeEnum::jmp:
            {
                switch (Opera->For_o_back)
                {
                    case 0:
                    {
                        ip += Opera->JMP_amount;
                        break;
                    }
                    case 1:
                    {
                        ip -= Opera->JMP_amount;
                        break;
                    }
                }
                break;
            }
            case ByteCodeEnum::add:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Op1Reg + Op2Reg;
                break;
            }
            case ByteCodeEnum::sub:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Op1Reg - Op2Reg;
                break;
            }
            case ByteCodeEnum::mul:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Op1Reg * Op2Reg;
                break;
            }
            case ByteCodeEnum::div:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Op1Reg / Op2Reg;
                break;
            }
            case ByteCodeEnum::pow:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Op1Reg.pow(Op2Reg);
            }
            case ByteCodeEnum::eq:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Constant{RendorConst{Op1Reg == Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::eq_not:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Constant{RendorConst{Op1Reg != Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::gr:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Constant{RendorConst{Op1Reg > Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::less:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Constant{RendorConst{Op1Reg < Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::gr_eq:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Constant{RendorConst{Op1Reg >= Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::less_eq:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type); auto& DstReg = (*DstRegisters)[Opera->Reg1];
                SetRegisters(&Op1Registers, Opera->Reg2_type); auto& Op1Reg = (*DstRegisters)[Opera->Reg2];
                SetRegisters(&Op2Registers, Opera->Reg3_type); auto& Op2Reg = (*DstRegisters)[Opera->Reg3];

                DstReg = Constant{RendorConst{Op1Reg <= Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            default:
            {
                throw error::RendorException("Unsupported instruction!");
            }
        }
    }
}
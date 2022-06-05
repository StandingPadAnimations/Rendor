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
                Stack[sp_int] = StackFrame{Opera->Reg1_16};
                Stack[sp_int].LastStackFrame = sp;
                sp = &Stack[sp_int];
                break;
            }
            case ByteCodeEnum::free:
            {
                --sp_int; // We can't do much ¯\_(ツ)_/¯
                sp = &Stack[sp_int];
                break;
            }
            case ByteCodeEnum::call:
            {
                auto FuncReg = CurrentModule->Functions[Opera->FunctionReg];
                SetRegisters(&DstRegisters, Opera->Ret_type); auto& DstReg = (*DstRegisters)[Opera->RetReg];
                sp->Ret = &DstReg;
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
            case ByteCodeEnum::ret:
            {
                if (Opera->Reg1_type_8 == 0) // no return value
                {
                    return;
                }

                // If we return a value
                SetRegisters(&DstRegisters, Opera->Reg2_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16]; // Get the register to return
                if (Opera->Reg3_type_8 == 0)
                {
                    *sp->Ret = std::move(DstReg);
                }
                else 
                {
                    *sp->Ret = DstReg.clone();
                }
                return; // Return from function
            }
            case ByteCodeEnum::mov:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8);
                SetRegisters(&Op1Registers, Opera->Reg2_type_8);

                // Move the value
                (*DstRegisters)[Opera->Reg1_16] = std::move((*Op1Registers)[Opera->Reg2_16]);
                break;
            }
            case ByteCodeEnum::mov_n:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); 
                (*DstRegisters)[Opera->Reg1_16] = std::move(Constant{RendorConst{false}, ConstType::NONE});
                break;
            }
            case ByteCodeEnum::cpy:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8);
                SetRegisters(&Op1Registers, Opera->Reg2_type_8);

                // Copy the value
                (*DstRegisters)[Opera->Reg1_16] = (*Op1Registers)[Opera->Reg2_16].clone();
                break;
            }
            case ByteCodeEnum::ref:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8);
                SetRegisters(&Op1Registers, Opera->Reg2_type_8);

                // Get value to reference
                auto& Val = (*Op1Registers)[Opera->Reg2_16];
                (*DstRegisters)[Opera->Reg1_16].Ref = &Val;
                break;
            }
            case ByteCodeEnum::stk_mov:
            {
                SetRegisters(&DstRegisters, Opera->Reg_64_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg_64];
                ConstStack.push_back(std::move(DstReg));
                break;
            }
            case ByteCodeEnum::stk_mov_n:
            {
                ConstStack.push_back(std::move(Constant{RendorConst{false}, ConstType::NONE}));
                break;
            }
            case ByteCodeEnum::stk_cpy:
            {
                SetRegisters(&DstRegisters, Opera->Reg_64_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg_64];
                ConstStack.push_back(std::move(DstReg.clone()));
                break;
            }
            case ByteCodeEnum::stk_pop:
            {
                ConstStack.pop_back();
                break;
            }
            case ByteCodeEnum::jmp:
            {
                switch (Opera->Reg_64_type_8)
                {
                    case 0:
                    {
                        ip += Opera->Reg_64;
                        break;
                    }
                    case 1:
                    {
                        ip -= Opera->Reg_64;
                        break;
                    }
                }
                break;
            }
            case ByteCodeEnum::icr:
            {
                SetRegisters(&DstRegisters, Opera->Reg_64_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg_64];
                auto Int = std::get_if<RendorInt>(&DstReg.Num);
                ++Int;
                break;
            }
            case ByteCodeEnum::dcr:
            {
                SetRegisters(&DstRegisters, Opera->Reg_64_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg_64];
                auto Int = std::get_if<RendorInt>(&DstReg.Num);
                --Int;
                break;
            }
            case ByteCodeEnum::add:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];

                DstReg = Op1Reg + Op2Reg;
                break;
            }
            case ByteCodeEnum::sub:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];

                DstReg = Op1Reg - Op2Reg;
                break;
            }
            case ByteCodeEnum::mul:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];

                DstReg = Op1Reg * Op2Reg;
                break;
            }
            case ByteCodeEnum::div:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Op1Reg / Op2Reg;
                break;
            }
            case ByteCodeEnum::pow:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];

                DstReg = Op1Reg.pow(Op2Reg);
                break;
            }
            case ByteCodeEnum::eq:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Constant{RendorConst{Op1Reg == Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::eq_not:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Constant{RendorConst{Op1Reg != Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::gr:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];

                DstReg = Constant{RendorConst{Op1Reg > Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::less:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];

                DstReg = Constant{RendorConst{Op1Reg < Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::gr_eq:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];

                DstReg = Constant{RendorConst{Op1Reg >= Op2Reg}, ConstType::CONST_BOOL};
                break;
            }
            case ByteCodeEnum::less_eq:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];

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
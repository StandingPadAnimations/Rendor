#include "RendorInterpreter/Interpreter.hpp"
#include <cstddef>
#include <variant>

void Interpreter::ExecuteByteCode(const std::vector<Operation>& Func, const std::size_t start)
{
    for (std::size_t ip = start; ip < Func.size(); ++ip)
    {
        const Operation* Opera = &Func[ip];
        Registers* DstRegisters = nullptr;
        Registers* Op1Registers = nullptr;
        Registers* Op2Registers = nullptr;
        switch (Opera->Operation)
        {
            case ByteCodeEnum::alloc:
            {
                if (m_sp_int == RENDOR_CALL_STACK_LIMIT)
                {
                    throw error::RendorException("Ran out of stack memory!");
                }
                ++m_sp_int;
                m_Stack[m_sp_int] = StackFrame{Opera->Reg_64_type_8};
                m_Stack[m_sp_int].LastStackFrame = m_sp;
                m_sp = &m_Stack[m_sp_int];
                break;
            }
            case ByteCodeEnum::free:
            {
                if (Opera->Reg1_type_8 == 0)
                {
                    --m_sp_int; // We can't do much ¯\_(ツ)_/¯
                    m_sp = &m_Stack[m_sp_int];
                }
                else if (Opera->Reg1_type_8 == 1)
                {
                    m_ModuleStack.pop_back();
                }
                break;
            }
            case ByteCodeEnum::mod_push:
            {
                m_CurrentModule = m_ModuleStack.emplace_back(m_Modules[Opera->Reg_64]);
                break;
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
                auto* Val = &(*Op1Registers)[Opera->Reg2_16];
                (*DstRegisters)[Opera->Reg1_16].Ref = Val;
                break;
            }
            case ByteCodeEnum::jmp:
            {
                switch (Opera->Reg_64_type_8)
                {
                    case 0:
                    {
                        ip = Opera->Reg_64;
                        break;
                    }
                    case 1:
                    {
                        ExecuteByteCode(m_ModuleStack.back()->Program, Opera->Reg_64);
                        break;
                    }
                }
                break;
            }
            case ByteCodeEnum::icr:
            {
                SetRegisters(&DstRegisters, Opera->Reg_64_type_8); const auto& DstReg = (*DstRegisters)[Opera->Reg_64];
                auto Int = std::get_if<RendorInt>(&DstReg.Num);
                ++Int;
                break;
            }
            case ByteCodeEnum::dcr:
            {
                SetRegisters(&DstRegisters, Opera->Reg_64_type_8); const auto& DstReg = (*DstRegisters)[Opera->Reg_64];
                auto Int = std::get_if<RendorInt>(&DstReg.Num);
                --Int;
                break;
            }
            case ByteCodeEnum::add:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Op1Reg + Op2Reg;
                break;
            }
            case ByteCodeEnum::sub:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Op1Reg - Op2Reg;
                break;
            }
            case ByteCodeEnum::mul:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Op1Reg * Op2Reg;
                break;
            }
            case ByteCodeEnum::div:
            {
                // Create and set the pointers
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
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
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Constant{RendorConst{Op1Reg == Op2Reg}, ConstType::CONST_BOOL}; const auto Res = std::get_if<bool>(&DstReg.Const);
                if (Res)
                {
                    ++ip;
                }
                break;
            }
            case ByteCodeEnum::eq_not:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Constant{RendorConst{Op1Reg != Op2Reg}, ConstType::CONST_BOOL}; const auto Res = std::get_if<bool>(&DstReg.Const);
                if (Res)
                {
                    ++ip;
                }
                break;
            }
            case ByteCodeEnum::gr:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Constant{RendorConst{Op1Reg > Op2Reg}, ConstType::CONST_BOOL}; const auto Res = std::get_if<bool>(&DstReg.Const);
                if (Res)
                {
                    ++ip;
                }
                break;
            }
            case ByteCodeEnum::less:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Constant{RendorConst{Op1Reg < Op2Reg}, ConstType::CONST_BOOL}; const auto Res = std::get_if<bool>(&DstReg.Const);
                if (Res)
                {
                    ++ip;
                }
                break;
            }
            case ByteCodeEnum::gr_eq:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Constant{RendorConst{Op1Reg >= Op2Reg}, ConstType::CONST_BOOL}; const auto Res = std::get_if<bool>(&DstReg.Const);
                if (Res)
                {
                    ++ip;
                }
                break;
            }
            case ByteCodeEnum::less_eq:
            {
                SetRegisters(&DstRegisters, Opera->Reg1_type_8); auto& DstReg = (*DstRegisters)[Opera->Reg1_16];
                SetRegisters(&Op1Registers, Opera->Reg2_type_8); const auto& Op1Reg = (*DstRegisters)[Opera->Reg2_16];
                SetRegisters(&Op2Registers, Opera->Reg3_type_8); const auto& Op2Reg = (*DstRegisters)[Opera->Reg3_16];
                DstReg = Constant{RendorConst{Op1Reg <= Op2Reg}, ConstType::CONST_BOOL}; const auto Res = std::get_if<bool>(&DstReg.Const);
                if (Res)
                {
                    ++ip;
                }
                break;
            }
            default:
            {
                throw error::RendorException("Unsupported instruction!");
            }
        }
    }
}
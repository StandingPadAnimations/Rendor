#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateVar(VariableType Type, std::string_view VariableName)
{
    const char* Variable = std::string(VariableName).c_str();
    asmjit::x86::Gp Arg = m_RendorJITCompiler.newGpd(Variable);
    asmjit::x86::Mem ConstPool;

    switch (Type)
    {
        case VariableType::Int:
        {
            int64_t Value = GrabInt64FromConsts_API();
            ConstPool = m_RendorJITCompiler.newInt64Const(asmjit::ConstPoolScope::kLocal, Value);
            m_RendorJITCompiler.mov(Arg, ConstPool);
            Variables[std::string{VariableName}] = std::make_pair(VariableType::Int, Arg);
            break;
        }
        case VariableType::Float:
        {
            double Value = GrabDoubleFromConsts_API();
            ConstPool = m_RendorJITCompiler.newDoubleConst(asmjit::ConstPoolScope::kLocal, Value);
            m_RendorJITCompiler.mov(Arg, ConstPool);
            Variables[std::string{VariableName}] = std::make_pair(VariableType::Float, Arg);
            break;
        }
        case VariableType::String:
        {
            const char* Value = GrabStringFromConsts_API();
            m_RendorJITCompiler.mov(Arg, Value);
            Variables[std::string{VariableName}] = std::make_pair(VariableType::Int, Arg);
            break;
        }
        case VariableType::Bool:
        {
            bool Value = GrabBoolFromConsts_API();
            ConstPool = m_RendorJITCompiler.newInt16Const(asmjit::ConstPoolScope::kLocal, Value);
            m_RendorJITCompiler.mov(Arg, ConstPool);
            Variables[std::string{VariableName}] = std::make_pair(VariableType::Bool, Arg);
            break;
        }
        default:
        {
            throw error::RendorException("RendorJIT argument error");
        }
    }
}
#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateFunction()
{
    m_RendorJITCodeHolder.attach(&m_RendorJITCompiler);
    m_RendorJITCompiler.addFunc(asmjit::FuncSignatureT<void>());
}

void RendorJIT::CreateVar(VariableType Type, std::string_view VariableName)
{
    const char* ArgsConstChar = std::string(VariableName).c_str();
    asmjit::x86::Gp Arg = m_RendorJITCompiler.newGpd(ArgsConstChar);
    asmjit::x86::Mem ConstPool;

    switch (Type)
    {
        case VariableType::Int:
        {
            int64_t Value = GrabInt64FromArgs_API();
            ConstPool = m_RendorJITCompiler.newDoubleConst(asmjit::ConstPoolScope::kLocal, Value);
            m_RendorJITCompiler.mov(Arg, ConstPool);
            Variables[std::string{VariableName}] = std::make_pair(VariableType::Int, Arg);
            break;
        }
        case VariableType::Float:
        {
            double Value = GrabDoubleFromArgs_API();
            ConstPool = m_RendorJITCompiler.newDoubleConst(asmjit::ConstPoolScope::kLocal, Value);
            m_RendorJITCompiler.mov(Arg, ConstPool);
            Variables[std::string{VariableName}] = std::make_pair(VariableType::Float, Arg);
            break;
        }
        case VariableType::String:
        {
            // ConstPool = m_RendorJITCompiler.newConst(asmjit::ConstPoolScope::kLocal, asmjit::String());
            // m_RendorJITCompiler.mov(Arg, ConstPool);
            Variables[std::string{VariableName}] = std::make_pair(VariableType::String, Arg);
            break;
        }
        case VariableType::Bool:
        {
            bool Value = GrabBoolFromArgs_API();
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

void RendorJIT::EndFunction()
{
    m_RendorJITCompiler.endFunc();
    m_RendorJITCompiler.finalize();

    /* ------------------------ Check if code is compiled ----------------------- */
    if (m_RendorJITCompiler.isFinalized())
    {
        RendorFunc* RendorJITFunc;
        asmjit::Error RendorJITAddFunc = m_RendorJITRuntime.add(&RendorJITFunc, &m_RendorJITCodeHolder);

        /* ------------- Check if there's an error when adding function ------------- */
        if (RendorJITAddFunc)
        {
            throw error::RendorException("RendorJIT function adding error: to avoid this please disable JIT");
        }
        
        /* -------------------------- reset the codeholder -------------------------- */
        m_RendorJITFunctions.push_back(RendorJITFunc);
        m_RendorJITCodeHolder.reset();
    }
    else 
    {
        throw error::RendorException("RendorJIT finalize error");
    }
}
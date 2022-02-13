#include "RendorInterpreter/RendorJit.hpp"

void RendorJIT::CreateFunction()
{
    m_RendorJITCodeHolder.attach(&m_RendorJITCompiler);
    m_RendorJITCompiler.addFunc(asmjit::FuncSignatureT<void>());
}

void RendorJIT::GenerateCode(std::string_view Command, std::string_view Args)
{
    if (Command == "FUNCTION")
    {
        if (Args == "END")
        {
            /* ------------------------ End function and compile ------------------------ */
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
    }
    else if ((Command == "ARGUMENT") || (Command == "ASSIGN"))
    {
        const char* ArgsConstChar = std::string(Args).c_str();
        asmjit::x86::Gp Arg = m_RendorJITCompiler.newGpd();
        asmjit::x86::Mem ConstPool;

        if (Command == "ARGUMENT")
        {
            switch ((*FunctionArgs)[FunctionArgIndex])
            {
                case VariableType::Int:
                {
                    ConstPool = m_RendorJITCompiler.newDoubleConst(asmjit::ConstPoolScope::kLocal, GrabInt64FromArgs_API());
                    m_RendorJITCompiler.mov(Arg, ConstPool);
                    Variables[Args] = std::make_pair(VariableType::Int, Arg);
                    break;
                }
                case VariableType::Float:
                {
                    ConstPool = m_RendorJITCompiler.newDoubleConst(asmjit::ConstPoolScope::kLocal, GrabDoubleFromArgs_API());
                    m_RendorJITCompiler.mov(Arg, ConstPool);
                    Variables[Args] = std::make_pair(VariableType::Float, Arg);
                    break;
                }
                case VariableType::String:
                {
                    ConstPool = m_RendorJITCompiler.newConst(asmjit::ConstPoolScope::kLocal, asmjit::String());
                    m_RendorJITCompiler.mov(Arg, ConstPool);
                    Variables[Args] = std::make_pair(VariableType::Float, Arg);
                    break;
                }
                case VariableType::Bool:
                {
                    ConstPool = m_RendorJITCompiler.newInt16Const(asmjit::ConstPoolScope::kLocal, GrabBoolFromArgs_API());
                    m_RendorJITCompiler.mov(Arg, ConstPool);
                    Variables[Args] = std::make_pair(VariableType::Float, Arg);
                    break;
                }
                default:
                {
                    throw error::RendorException("RendorJIT argument error");
                }
            }
        }
        else 
        {
            switch (GrabPointedConstType_API())
            {
                case VariableType::Int:
                {
                    ConstPool = m_RendorJITCompiler.newDoubleConst(asmjit::ConstPoolScope::kLocal, GrabInt64FromConsts_API());
                    Variables[Args] = std::make_pair(VariableType::Int, Arg);
                    m_RendorJITCompiler.mov(Arg, ConstPool);
                    break;
                }
                case VariableType::Float:
                {
                    ConstPool = m_RendorJITCompiler.newDoubleConst(asmjit::ConstPoolScope::kLocal, GrabDoubleFromConsts_API());
                    m_RendorJITCompiler.mov(Arg, ConstPool);
                    Variables[Args] = std::make_pair(VariableType::Float, Arg);
                    break;
                }
                case VariableType::String:
                {
                    ConstPool = m_RendorJITCompiler.newConst(asmjit::ConstPoolScope::kLocal, GrabStringFromConsts_API());
                    m_RendorJITCompiler.mov(Arg, ConstPool);
                    Variables[Args] = std::make_pair(VariableType::Float, Arg);
                    break;
                }
                case VariableType::Bool:
                {
                    ConstPool = m_RendorJITCompiler.newInt16Const(asmjit::ConstPoolScope::kLocal, GrabBoolFromConsts_API());
                    m_RendorJITCompiler.mov(Arg, ConstPool);
                    Variables[Args] = std::make_pair(VariableType::Float, Arg);
                    break;
                }
                default:
                {
                    throw error::RendorException("RendorJIT argument error");
                }
            }
        }
    }
}
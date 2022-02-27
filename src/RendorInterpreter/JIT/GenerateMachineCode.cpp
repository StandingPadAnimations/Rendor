#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::GenerateFunction()
{
    for (auto const& IR_Op : (*m_IR))
    {
        size_t IndexOfUnderScore1 = IR_Op.find_first_of("_");
        string_view Type = IR_Op.substr(0, IndexOfUnderScore1);
        IRType Operation = m_IRtype[Type];

        switch (Operation)
        {
            case IRType::FUNC:
            {
                m_RendorJITCompiler.newFunc(asmjit::FuncSignatureT<void>());
                break;
            }

            case IRType::VAR:
            {
                size_t IndexOfEqual1 = IR_Op.find_first_of("=", IndexOfUnderScore1);
                string Value = std::string{IR_Op.substr(IndexOfEqual1)};
                asmjit::x86::Gp Var;

                switch (Variables[IR_Op].first)
                {
                    case VariableType::Int64:
                    {
                        asmjit::x86::Mem vConst = m_RendorJITCompiler.newInt64Const(asmjit::ConstPoolScope::kLocal, boost::lexical_cast<int64_t>(Value));
                        m_RendorJITCompiler.mov(Var, vConst);
                    }
                    case VariableType::Double:
                    {
                        asmjit::x86::Mem vConst = m_RendorJITCompiler.newDoubleConst(asmjit::ConstPoolScope::kLocal, boost::lexical_cast<double>(Value));
                        m_RendorJITCompiler.mov(Var, vConst);
                    }
                    case VariableType::String:
                    {
                        asmjit::x86::Mem vConst = m_RendorJITCompiler.newInt64Const(asmjit::ConstPoolScope::kLocal, boost::lexical_cast<double>(Value));
                        m_RendorJITCompiler.mov(Var, vConst);
                    }
                    case VariableType::Bool:
                    {
                        asmjit::x86::Mem vConst = m_RendorJITCompiler.newInt16Const(asmjit::ConstPoolScope::kLocal, boost::lexical_cast<bool>(Value));
                        m_RendorJITCompiler.mov(Var, vConst);
                    }
                    default:
                    {
                        throw error::RendorException("Not supported type in JIT: " + std::string{IR_Op});
                    }
                }
                Variables[IR_Op].second = Var;
            }

            default:
            {
                throw error::RendorException("Not supported in JIT: " + std::string{IR_Op});
            }
        }
    }
}
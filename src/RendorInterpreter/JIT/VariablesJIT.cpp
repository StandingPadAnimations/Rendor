#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateVarRepr(VariableType Type, std::string_view VariableName)
{
    switch (Type)
    {
        case VariableType::Int:
        {
            m_IR.emplace_back("_v_i64_" + std::string{VariableName});
            break;
        }
        case VariableType::Float:
        {
            m_IR.emplace_back("_v_d_" + std::string{VariableName});
            break;
        }
        case VariableType::String:
        {
            m_IR.emplace_back("_v_s_" + std::string{VariableName});
            break;
        }
        case VariableType::Bool:
        {
            m_IR.emplace_back("_v_b_" + std::string{VariableName});
            break;
        }
        default:
        {
            throw error::RendorException("RendorJIT argument error");
        }
    }
}
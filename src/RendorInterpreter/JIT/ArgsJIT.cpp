#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateArgRepr(VariableType Type, std::string_view VariableName)
{
    switch (Type)
    {
        case VariableType::Int:
        {
            m_IR.emplace_back("_a_i64_" + std::string{VariableName});
            break;
        }
        case VariableType::Float:
        {
            m_IR.emplace_back("_a_d_" + std::string{VariableName});
            break;
        }
        case VariableType::String:
        {
            m_IR.emplace_back("_a_s_" + std::string{VariableName});
            break;
        }
        case VariableType::Bool:
        {
            m_IR.emplace_back("_a_b_" + std::string{VariableName});
            break;
        }
        default:
        {
            throw error::RendorException("RendorJIT argument error");
        }
    }
}

#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateArgRepr(VariableType Type, std::string& VariableName)
{
    switch (Type)
    {
        case VariableType::Int64:
        {
            m_IR->emplace_back("arg_i64_" + VariableName);
            break;
        }
        case VariableType::Double:
        {
            m_IR->emplace_back("arg_d_" + VariableName);
            break;
        }
        case VariableType::String:
        {
            m_IR->emplace_back("arg_s_" + VariableName);
            break;
        }
        case VariableType::Bool:
        {
            m_IR->emplace_back("arg_b_" + VariableName);
            break;
        }
        default:
        {
            throw error::RendorException("RendorJIT argument error");
        }
    }
    Variables[m_IR->back()].first = Type;
}

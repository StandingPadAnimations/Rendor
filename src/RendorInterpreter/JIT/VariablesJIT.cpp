#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateVarRepr(VariableType Type, std::string& VariableName)
{
    switch (Type)
    {
        case VariableType::Int64:
        {
            m_IR->emplace_back("var_" + VariableName + "=" + std::to_string(GrabInt64FromConsts_API()).c_str());
            break;
        }
        case VariableType::Double:
        {
            m_IR->emplace_back("var_" + VariableName+ "=" + std::to_string(GrabDoubleFromConsts_API()).c_str());
            break;
        }
        case VariableType::String:
        {
            m_IR->emplace_back("var_" + VariableName+ "=" + GrabStringFromConsts_API());
            break;
        }
        case VariableType::Bool:
        {
            m_IR->emplace_back("var_" + VariableName+ "=" + std::to_string(GrabBoolFromConsts_API()).c_str());
            break;
        }
        default:
        {
            throw error::RendorException("RendorJIT variable error");
        }
    }
    Variables[VariableName].first = Type;
}
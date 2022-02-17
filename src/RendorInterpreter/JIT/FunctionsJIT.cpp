#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateFunctionRepr(std::string_view Name, std::initializer_list<std::string_view> Args)
{
    std::string Function = "_" + std::string{Name};
    for (auto const& Argument : Args)
    {
        Function += "_" + std::string{Argument};
    }
    m_IR.emplace_back(Function);
}

void RendorJIT::EndFunctionRepr(std::string_view Return)
{
    m_IR.emplace_back("func_end_" + std::string{Return});
}


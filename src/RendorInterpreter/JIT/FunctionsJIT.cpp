#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateFunctionRepr(std::string_view Name, std::initializer_list<std::string_view> Args)
{
    m_IR->emplace_back("func_" + std::string{Name});
}

void RendorJIT::EndFunctionRepr(std::string_view Return)
{
    m_IR->emplace_back("ret_" + std::string{Return});
}


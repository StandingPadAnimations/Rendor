#include "RendorInterpreter/RendorJit.hpp"

void RendorJit::CreateFunction(std::string_view Name, std::vector<RendorJitArgTypes>& Types)
{
    asmjit::x86::Compiler RendorJitCompiler(&m_RendorJitCodeHolder);
    
}
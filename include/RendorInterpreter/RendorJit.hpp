#ifndef RENDOR_JIT_HPP
#define RENDOR_JIT_HPP

#include <string_view>
#include <vector>
#include <map>
#include <

#define ASMJIT_STATIC
#include "../src/asmjit/x86.h"

enum class RendorJitArgTypes
{
    INT_64,
    DOUBLE,
    STRING,
    BOOL
};

class RendorJit
{
    public:
        RendorJit()
        {
            m_RendorJitCodeHolder.init(m_RendorJitRuntime.environment());
        }

    private:
        static asmjit::JitRuntime m_RendorJitRuntime;
        static asmjit::CodeHolder m_RendorJitCodeHolder;

        static void CreateFunction(std::string_view Name, std::vector<RendorJitArgTypes>& Types);
};


#endif // RENDOR_JIT_HPP
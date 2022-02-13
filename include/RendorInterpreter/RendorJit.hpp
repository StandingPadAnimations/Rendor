#ifndef RENDOR_JIT_HPP
#define RENDOR_JIT_HPP

#include <string_view>
#include <vector>
#include <map>
#include <functional>

#include "RendorInterpreter/RendorTypes.hpp"
#include "RendorInterpreter/RendorAPI.hpp"
#include "RendorInterpreter/RendorInternalAPI.hpp"
#include "Exceptions.hpp"

#define ASMJIT_STATIC
#include "../src/asmjit/x86.h"

typedef void (*RendorFunc)(void);

class RendorJITErrorHandeler : public asmjit::ErrorHandler 
{
    public:
        void handleError(asmjit::Error err, const char* message, asmjit::BaseEmitter* origin) 
        override 
        {
            printf("AsmJit error: %s\n", message);
        }
};

struct RendorJIT
{
    static asmjit::JitRuntime m_RendorJITRuntime;
    static asmjit::CodeHolder m_RendorJITCodeHolder;
    static asmjit::x86::Compiler m_RendorJITCompiler;
    static RendorJITErrorHandeler m_RendorJITErrorHandeler;
    static std::vector<RendorFunc*> m_RendorJITFunctions;
    static std::map<std::string_view, std::pair<VariableType, asmjit::x86::Gp>> Variables;

    RendorJIT()
    {
        m_RendorJITCodeHolder.init(m_RendorJITRuntime.environment());
        m_RendorJITCodeHolder.setErrorHandler(&m_RendorJITErrorHandeler);
    }

    inline static size_t FunctionArgIndex = 0;
    static std::vector<VariableType>* FunctionArgs;
    static void CreateFunction();
    static void GenerateCode(std::string_view Command, std::string_view Args);
    static RendorFunc* GetGeneratedFunction();
};


#endif // RENDOR_JIT_HPP
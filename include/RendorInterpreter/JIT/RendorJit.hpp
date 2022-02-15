#ifndef RENDOR_JIT_HPP
#define RENDOR_JIT_HPP

#include <string_view>
#include <vector>
#include <unordered_map>

using std::string_view;
using std::vector;
using std::pair;
using std::unordered_map; 

#include "RendorInterpreter/Resources/RendorTypes.hpp"
#include "RendorInterpreter/API/RendorAPI.hpp"
#include "RendorInterpreter/API/RendorInternalAPI.hpp"
#include "RendorInterpreter/Resources/RendorDefinitions.hpp"
#include "Exceptions.hpp"
#include "UnorderedMapLookUp.hpp"

#define ASMJIT_STATIC
#include "../src/asmjit/x86.h"


class RendorJITErrorHandeler : public asmjit::ErrorHandler 
{
    public:
        void handleError([[maybe_unused]] asmjit::Error err, [[maybe_unused]] const char* message, [[maybe_unused]] asmjit::BaseEmitter* origin) 
        override 
        {
            throw error::RendorException("AsmJit error: " + std::string{message});
        }
};

struct RendorJIT
{
    /* ------------------------------ asmjit stuff ------------------------------ */
    asmjit::JitRuntime m_RendorJITRuntime;
    asmjit::CodeHolder m_RendorJITCodeHolder;
    asmjit::x86::Compiler m_RendorJITCompiler;
    RendorJITErrorHandeler m_RendorJITErrorHandeler;

    /* ------------------------- Related to the function ------------------------ */
    size_t FunctionArgIndex = 0;
    std::vector<VariableType>* FunctionArgs;

    /* --------------------------------- Storage -------------------------------- */
    std::vector<RendorFunc*> m_RendorJITFunctions;
    std::unordered_map<std::string, std::pair<VariableType, asmjit::x86::Gp>, string_hash, std::equal_to<>> Variables;

    RendorJIT()
    {
        m_RendorJITCodeHolder.init(m_RendorJITRuntime.environment());
        m_RendorJITCodeHolder.setErrorHandler(&m_RendorJITErrorHandeler);
    }

    /* ---------------------------- Get new function ---------------------------- */
    RendorFunc* GetGeneratedFunction();

    /* ------------------------------- Generation ------------------------------- */
    void CreateFunction();
    void CreateVar(VariableType Type, std::string_view VariableName);
    void EndFunction();
    private:
};


#endif // RENDOR_JIT_HPP
#ifndef RENDOR_JIT_HPP
#define RENDOR_JIT_HPP

#include <string_view>
#include <vector>
#include <unordered_map>
#include <initializer_list>

using std::initializer_list;
using std::string_view;
using std::vector;
using std::pair;
using std::unordered_map; 

#include "RendorInterpreter/Resources/RendorTypes.hpp"
#include "RendorInterpreter/API/RendorAPI.hpp"
#include "RendorInterpreter/API/RendorInternalAPI.hpp"
#include "RendorInterpreter/Resources/RendorDefinitions.hpp"
#include "RendorInterpreter/Resources/RendorTypes.hpp"
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

class RendorJIT
{
    public:
        /* ------------------------------ asmjit stuff ------------------------------ */
        asmjit::JitRuntime m_RendorJITRuntime;
        asmjit::CodeHolder m_RendorJITCodeHolder;
        asmjit::x86::Compiler m_RendorJITCompiler;
        RendorJITErrorHandeler m_RendorJITErrorHandeler;

        /* ------------------------- Related to the function ------------------------ */
        size_t FunctionArgIndex = 0;
        std::vector<VariableType> FunctionArgs;

        /* --------------------------------- Storage -------------------------------- */
        std::vector<RendorFunc*> m_RendorJITFunctions;
        std::unordered_map<std::string, std::pair<VariableType, asmjit::x86::Gp>, string_hash, std::equal_to<>> Variables;

        /* ----------------------------------- IR ----------------------------------- */
        std::vector<std::string> m_IR;

        RendorJIT()
        {
            m_RendorJITCodeHolder.init(m_RendorJITRuntime.environment());
            m_RendorJITCodeHolder.setErrorHandler(&m_RendorJITErrorHandeler);
        }

        /* ---------------------------- Get new function ---------------------------- */
        RendorFunc* GetGeneratedFunction();

        /* ------------------------------- Generation ------------------------------- */
        void CreateFunctionRepr(std::string_view Name, std::initializer_list<std::string_view> Args);
        void EndFunctionRepr(std::string_view Return);
        void CreateVarRepr(VariableType Type, std::string_view VariableName);
        void CreateArgRepr(VariableType Type, std::string_view VariableName);
        void CreateIfElseRepr(std::string_view Const1, std::string_view Const2, Operator ComparisonOperator);

    private:
        void CreateFunction(std::string_view Repr);
        void EndFunction(std::string_view Repr);
        void CreateVar(std::string_view Repr);
        void CreateArg(std::string_view Repr);
        void CreateIfElse(std::string_view Repr);
};


#endif // RENDOR_JIT_HPP
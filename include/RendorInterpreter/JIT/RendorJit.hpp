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


enum class IRType
{
    FUNC,
    VAR,
    ARG,
    RET,
    IF,
    ELSE,
    INCR,
    DECR
};

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
        /* --------------------------------- Storage -------------------------------- */
        std::vector<RendorFunc*> m_RendorJITFunctions;
        
        RendorJIT()
        {
            m_RendorJITCodeHolder.init(m_RendorJITRuntime.environment());
            m_RendorJITCodeHolder.setErrorHandler(&m_RendorJITErrorHandeler);
        }

        /* ------------------------------- Generation ------------------------------- */
        void CreateFunctionRepr(std::string_view Name, std::initializer_list<std::string_view> Args);
        void EndFunctionRepr(std::string_view Return);
        void CreateVarRepr(VariableType Type, std::string& VariableName);
        void CreateArgRepr(VariableType Type, std::string& VariableName);
        void CreateIfElseRepr(std::string_view Const1, std::string_view Const2, Operator ComparisonOperator);

        void GenerateFunction();

    private:
        /* ------------------------------ asmjit stuff ------------------------------ */
        asmjit::JitRuntime m_RendorJITRuntime;
        asmjit::CodeHolder m_RendorJITCodeHolder;
        asmjit::x86::Compiler m_RendorJITCompiler;
        RendorJITErrorHandeler m_RendorJITErrorHandeler;

        /* ----------------------------------- IR ----------------------------------- */
        std::vector<std::string>* m_IR = nullptr;
        std::unordered_map<std::string_view, std::pair<VariableType, asmjit::x86::Gp>, string_hash, std::equal_to<>> Variables;
        std::unordered_map<std::string_view, IRType, string_hash, std::equal_to<>> m_IRtype
        {
            {"func", IRType::FUNC},
            {"arg",  IRType::ARG},
            {"ret",  IRType::RET},
            {"if",   IRType::IF},
            {"else", IRType::ELSE},
            {"var",  IRType::VAR},
            {"incr", IRType::INCR},
            {"decr", IRType::DECR},
        };
};


#endif // RENDOR_JIT_HPP
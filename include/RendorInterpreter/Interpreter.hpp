#ifndef RENDOR_INTERPRETER_HPP
#define RENDOR_INTERPRETER_HPP

#define RENDOR_CALL_STACK_LIMIT 100

#include <fmt/core.h>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include <stack>

#include "RendorInterpreter/Objects/Constant.hpp"
#include "RendorInterpreter/Objects/Stackframe.hpp"
#include "RendorInterpreter/Objects/Modules.hpp"
#include "RendorAPI/RendorAPI.h"
#include "CrenParsing/Header.hpp"
#include "CrenParsing/Operation.hpp"

#include "Definitions.hpp"
#include "UnorderedMapLookUp.hpp"
#include "Exceptions.hpp"

#include <binary_io/binary_io.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

class Interpreter
{
    public:
        explicit Interpreter(binary_io::file_istream& t_File) noexcept
        {
            m_File = &t_File;
        };

        bool PrepareInterpreter();
        void ExecuteMain()
        {
            ExecuteByteCode(m_CurrentModule->Program, 0);
        }

        /* ------------------------------ API functions ----------------------------- */
        int64_t GrabInt64FromStack();
        double GrabDoubleFromStack();
        const char* GrabConstCharFromStack();
        const char* GrabConstFromStackAsConstChar();
        bool GrabBoolFromStack();
        TypeEnum GrabObjTypeFromStack();

        void DropInt64OnStack(int64_t Const);
        void DropDoubleOnStack(double Const);
        void DropConstCharOnStack(const char* Const);
        void DropBoolOnStack(bool Const);
        void PopStack();

        void InitModule(RendorMethod *MethodList);
        void ThrowStackTrace() const noexcept
        {
            for (const auto& err : m_Stacktrace)
            {
                fmt::print("{}\n", err);
            }
        }

    private:
        std::array<StackFrame, RENDOR_CALL_STACK_LIMIT> m_Stack;
        std::vector<Module*> m_ModuleStack;
        StackFrame* m_sp = nullptr;
        std::size_t m_sp_int = 0;
        
        binary_io::file_istream* m_File;
        Module* m_CurrentModule = nullptr;
        CrenHeader m_Header;

        std::vector<Module> m_Modules;

        std::vector<std::string> m_Stacktrace;

        void CreateConstPool();
        void CreateStrConstPool();
        void ImportModules(){}; // ! UNSUPPORTED RIGHT NOW
        void ReadFunctions();
        void ExecuteByteCode(const std::vector<Operation>& Func, const std::size_t start);

        /* ---------------------------- Helper functions ---------------------------- */
        void AddModule()
        {
            m_Modules.push_back(Module{});
            m_CurrentModule = &m_Modules.back();
            m_CurrentModule->Pool = ConstPool{};
            m_CurrentModule->GlobalVars = StackFrame{};
        }

        void SetRegisters(Registers** Ptr, std::uint8_t Type)
        {
            switch (Type)
            {
                case 0:
                {
                    *Ptr = &m_CurrentModule->Pool.Pool;
                    break;
                }
                case 1:
                {
                    *Ptr = &m_CurrentModule->GlobalVars.Registers;
                    break;
                }
                case 2:
                {
                    *Ptr = &m_sp->LastStackFrame->Registers;
                    break;
                }
                case 3:
                {
                    *Ptr = &m_sp->Registers;
                    break;
                }
                default:
                {
                    throw RENDOR_SEG_FAULT;
                }
            }
        }
};

#endif // RENDOR_INTERPRETER_HPP
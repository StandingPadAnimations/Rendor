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

#include "Definitions.hpp"
#include "UnorderedMapLookUp.hpp"
#include "Exceptions.hpp"

#include <binary_io/binary_io.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

class Interpreter
{
    public:
        explicit Interpreter(binary_io::file_istream& Input)
        {
            File = &Input;
        };

        bool PrepareInterpreter();
        void ExecuteMain()
        {
            ExecuteByteCode(CurrentModule->Program);
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
        void ThrowStackTrace()
        {
            for (const auto& err : Stacktrace)
            {
                fmt::print("{}\n", err);
            }
        }

    private:
        std::array<StackFrame, RENDOR_CALL_STACK_LIMIT> Stack;
        StackFrame* sp = nullptr;
        std::size_t sp_int = 0;
        
        binary_io::file_istream* File;
        Module* CurrentModule = nullptr;
        CrenHeader header;

        std::vector<Module> Modules;

        std::vector<std::string> Stacktrace;

        void CreateConstPool();
        void CreateStrConstPool();
        void ImportModules(){}; // ! UNSUPPORTED RIGHT NOW
        void ReadFunctions();
        void ExecuteByteCode(const std::vector<Operation>& Func);

        /* ---------------------------- Helper functions ---------------------------- */
        void AddModule()
        {
            Modules.push_back(Module{});
            CurrentModule = &Modules.back();
            CurrentModule->Pool = ConstPool{};
            CurrentModule->GlobalVars = StackFrame{};
        }

        void SetRegisters(Registers** Ptr, std::uint8_t Type)
        {
            switch (Type)
            {
                case 0:
                {
                    *Ptr = &CurrentModule->Pool.Pool;
                    break;
                }
                case 1:
                {
                    *Ptr = &CurrentModule->GlobalVars.Registers;
                    break;
                }
                case 2:
                {
                    *Ptr = &sp->LastStackFrame->Registers;
                    break;
                }
                case 3:
                {
                    *Ptr = &sp->Registers;
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
#ifndef RENDOR_INTERPRETER_HPP
#define RENDOR_INTERPRETER_HPP

#include <vector>
#include <array>
#include <memory>

#include "RendorInterpreter/Objects/Constant.hpp"
#include "RendorInterpreter/Objects/Stackframe.hpp"
#include "RendorAPI/RendorAPI.h"

#include "Definitions.hpp"
#include "UnorderedMapLookUp.hpp"
#include "Exceptions.hpp"

#include <boost/iostreams/device/mapped_file.hpp>

class Interpreter
{
    public:
        Interpreter()
        {
            Stack[0] = StackFrame();
            CurrentStackFrame = &Stack[0];
        };
        void ExecuteByteCode(const boost::iostreams::mapped_file& File);

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

    private:
        std::array<StackFrame, 100> Stack;
        StackFrame* CurrentStackFrame = nullptr;
};

#endif // RENDOR_INTERPRETER_HPP
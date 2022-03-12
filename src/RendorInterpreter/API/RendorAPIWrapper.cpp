#include "RendorInterpreter/Interpreter.hpp"
#include "RendorAPI/RendorAPI.h"

/* -------------------------------------------------------------------------- */
/*                        Grabbing values off the stack                       */
/* -------------------------------------------------------------------------- */
int64_t Rendor_GrabInt64FromStack()
{
    int64_t Result = Interpreter::GrabInt64FromStack();
    Interpreter::PopStack();
    return Result;
}
double Rendor_GrabDoubleFromStack()
{
    double Result = Interpreter::GrabDoubleFromStack();
    Interpreter::PopStack();
    return Result;
}
const char* Rendor_GrabConstCharFromStack()
{
    const char* Result = Interpreter::GrabConstCharFromStack();
    Interpreter::PopStack();
    return Result;
}
const char* Rendor_GrabConstFromStackAsConstChar()
{
    const char* Result = Interpreter::GrabConstFromStackAsConstChar();
    Interpreter::PopStack();
    return Result;
}
int Rendor_GrabBoolFromStack()
{
    int Result = Interpreter::GrabBoolFromStack();
    Interpreter::PopStack();
    return Result;
}
TypeEnum Rendor_GrabObjTypeFromStack()
{
    switch (Interpreter::GrabObjTypeFromStack())
    {
        case VariableType::Int64:
        {
            return TypeEnum::INT64;
        }
        case VariableType::Double:
        {
            return TypeEnum::DOUBLE;
        }
        case VariableType::String:
        {
            return TypeEnum::STRING;
        }
        case VariableType::Bool:
        {
            return TypeEnum::BOOL;
        }
        default:
        {
            break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                            Dropping on the stack                           */
/* -------------------------------------------------------------------------- */
void Rendor_DropInt64OnStack(int64_t Const)
{
    Interpreter::DropInt64OnStack(Const);
}
void Rendor_DropDoubleOnStack(double Const)
{
    Interpreter::DropDoubleOnStack(Const);
}
void Rendor_DropConstCharOnStack(const char* Const)
{
    Interpreter::DropConstCharOnStack(Const);
}
void Rendor_DropBoolOnStack(int Const)
{
    Interpreter::DropBoolOnStack(Const);
}
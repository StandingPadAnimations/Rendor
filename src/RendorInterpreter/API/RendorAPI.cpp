#include "RendorInterpreter/Interpreter.hpp"


/* -------------------------------------------------------------------------- */
/*                                 API Methods                                */
/* -------------------------------------------------------------------------- */
int64_t Interpreter::GrabInt64FromStack()
{
    const auto CopiedVariable = std::get<0>(RendorStack.top());
    auto& IntObj = static_cast<Int64&>(*CopiedVariable);
    return IntObj.ConvertedValue;
}
double Interpreter::GrabDoubleFromStack()
{
    const auto CopiedVariable = std::get<0>(RendorStack.top());
    auto& DoubleObj = static_cast<Double&>(*CopiedVariable);
    return DoubleObj.ConvertedValue;
}

const char* Interpreter::GrabConstCharFromStack()
{
    const auto CopiedVariable = std::get<0>(RendorStack.top());
    auto& ConstCharObj = static_cast<String&>(*CopiedVariable);
    return ConstCharObj.m_Value.c_str();
}

const char* Interpreter::GrabConstFromStackAsConstChar()
{
    const auto CopiedVariable = std::get<0>(RendorStack.top());
    return CopiedVariable->m_Value.c_str();
}

bool Interpreter::GrabBoolFromStack()
{
    const auto CopiedVariable = std::get<0>(RendorStack.top());
    auto& BoolObj = static_cast<Bool&>(*CopiedVariable);
    return BoolObj.ConvertedValue;
}

VariableType Interpreter::GrabObjTypeFromStack()
{
    const auto CopiedVariable = std::get<0>(RendorStack.top());
    return CopiedVariable->TypeOfVariable();
}

void Interpreter::PopStack()
{
    RendorStack.pop();
}

void Interpreter::DropInt64OnStack(int64_t Const)
{
    TypeObject_U Object = std::make_unique<Int64>(std::to_string(Const));
    RendorStack.emplace(Object.get(), std::move(Object));
}

void Interpreter::DropDoubleOnStack(double Const)
{
    TypeObject_U Object = std::make_unique<Double>(std::to_string(Const));
    RendorStack.emplace(Object.get(), std::move(Object));
}

void Interpreter::DropConstCharOnStack(const char* Const)
{
    TypeObject_U Object = std::make_unique<String>(Const);
    RendorStack.emplace(Object.get(), std::move(Object));
}

void Interpreter::DropBoolOnStack(bool Const)
{
    TypeObject_U Object = std::make_unique<Bool>(std::to_string(Const));
    RendorStack.emplace(Object.get(), std::move(Object));
}


/* -------------------------------------------------------------------------- */
/*                              Module functions                              */
/* -------------------------------------------------------------------------- */

void Interpreter::InitModule(RendorMethod *MethodList)
{
    bool IterateArray = false;
    size_t Index = 0;
    do 
    {
        RendorMethod Method = MethodList[Index];
        if (Method.Name == NULL)
        {
            IterateArray = false;
        }

        CppFunctions.emplace(Method.Name, Method.RendorFunc);
    } while (IterateArray);
}

#include "RendorInterpreter/Interpreter.hpp"

/*
    This file is used to define API functions for the interpreter functions themsevles. 
    For how the wrappers are coded, see RendorAPI.cpp
*/

/* ---------------------------- Dispose constants --------------------------- */
void Interpreter::DisposeConst(TypeObject Const)
{
    switch (Const->ColorOfObj)
    {
        case GCColor::BLACK:
        {
            auto OriginalPtr = std::find(BlackObjects.begin(), BlackObjects.end(), Const);
            WhiteObjects.push_back(std::move(*OriginalPtr));
            BlackObjects.erase(OriginalPtr);
            break;
        }

        case GCColor::GREY:
        {
            auto OriginalPtr = std::find(GreyObjects.begin(), GreyObjects.end(), Const);
            WhiteObjects.push_back(std::move(*OriginalPtr));
            GreyObjects.erase(OriginalPtr);
            break;
        }

        default:
        {
            break;
        }
    }
    Const->ColorOfObj = GCColor::WHITE;
}

/* -------------------------------- Arguments ------------------------------- */
int64_t Interpreter::GrabInt64FromArgs()
{
    TypeObject Const = FunctionArgsCallStack.back().back().lock();
    auto& ConvertedObject = static_cast<Int&>(*Const);
    return ConvertedObject.ConvertedValue;
}
double Interpreter::GrabDoubleFromArgs()
{
    TypeObject Const = FunctionArgsCallStack.back().back().lock();
    auto& ConvertedObject = static_cast<Float&>(*Const);
    return ConvertedObject.ConvertedValue;
}
const char* Interpreter::GrabStringFromArgs()
{
    TypeObject Const = FunctionArgsCallStack.back().back().lock();
    auto& ConvertedObject = static_cast<String&>(*Const);
    return ConvertedObject.m_Value.c_str();
}
bool Interpreter::GrabBoolFromArgs()
{
    TypeObject Const = FunctionArgsCallStack.back().back().lock();
    auto& ConvertedObject = static_cast<Bool&>(*Const);
    return ConvertedObject.ConvertedValue;
}

/* -------------------------------- Constants ------------------------------- */
VariableType Interpreter::GrabPointedType()
{
    return Constants[ConstantIndex].lock()->TypeOfVariable();
}
int64_t Interpreter::GrabInt64FromConsts()
{
    TypeObject Const = Constants[ConstantIndex].lock();
    auto& ConvertedObject = static_cast<Int&>(*Const);
    return ConvertedObject.ConvertedValue;
}
double Interpreter::GrabDoubleFromConsts()
{
    TypeObject Const = Constants[ConstantIndex].lock();
    auto& ConvertedObject = static_cast<Float&>(*Const);
    return ConvertedObject.ConvertedValue;
}
const char* Interpreter::GrabStringFromConsts()
{
    TypeObject Const = Constants[ConstantIndex].lock();
    auto& ConvertedObject = static_cast<String&>(*Const);
    return ConvertedObject.m_Value.c_str();
}
bool Interpreter::GrabBoolFromConsts()
{
    TypeObject Const = Constants[ConstantIndex].lock();
    auto& ConvertedObject = static_cast<Bool&>(*Const);
    return ConvertedObject.ConvertedValue;
}
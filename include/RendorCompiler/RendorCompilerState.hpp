#ifndef RENDOR_COMPILER_STATE_HPP
#define RENDOR_COMPILER_STATE_HPP
#include "RendorCompiler/ASTInspection/ASTInspector.hpp"
#include "RendorAPI/RendorAPI.hpp"

struct CompilerState : RendorState
{
    virtual int64_t Rendor_GrabInt64FromStack() override {return 0;};
    virtual double Rendor_GrabDoubleFromStack() override {return 0.0;};
    virtual const char* Rendor_GrabConstCharFromStack() override {return "";};
    virtual const char* Rendor_GrabConstFromStackAsConstChar() override {return "";};
    virtual TypeEnum Rendor_GrabObjTypeFromStack() override {return C_INT64;};
    virtual bool Rendor_GrabBoolFromStack() override {return false;};
    virtual void Rendor_DropInt64OnStack([[maybe_unused]] int64_t Const) override {};
    virtual void Rendor_DropDoubleOnStack([[maybe_unused]] double Const) override {};
    virtual void Rendor_DropConstCharOnStack([[maybe_unused]] const char* Const) override {};
    virtual void Rendor_DropBoolOnStack([[maybe_unused]] int Const) override {};
    virtual void Rendor_InitModule(RendorMethod *MethodList) override {ASTInspector::InitModule(MethodList);};
    virtual bool Rendor_ConstEval() override {return true;};
};
#endif // RENDOR_COMPILER_STATE_HPP
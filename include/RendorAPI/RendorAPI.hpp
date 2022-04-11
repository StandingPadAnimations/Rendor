#ifndef RENDOR_API_H
#define RENDOR_API_H
#include <stdint.h>

#if __cplusplus 
    #ifdef __WIN32
        #define RENDOR_INIT extern "C" __declspec(dllexport) void
    #endif
    #define EXTERNAL extern "C"
#else
    #define RENDOR_INIT void
    #define EXTERNAL
#endif

EXTERNAL struct RendorState;
enum TypeEnum
{
    C_INT64,
    C_DOUBLE,
    C_STRING,
    C_BOOL
};

EXTERNAL struct RendorMethod
{
    const char* Name;
    size_t ArgCount;
    void (*RendorFunc)(RendorState*);
};

EXTERNAL struct RendorState
{
    virtual int64_t Rendor_GrabInt64FromStack() = 0;
    virtual double Rendor_GrabDoubleFromStack() = 0;
    virtual const char* Rendor_GrabConstCharFromStack() = 0;
    virtual const char* Rendor_GrabConstFromStackAsConstChar() = 0;
    virtual TypeEnum Rendor_GrabObjTypeFromStack() = 0;
    virtual int Rendor_GrabBoolFromStack() = 0;
    virtual void Rendor_DropInt64OnStack(int64_t Const) = 0;
    virtual void Rendor_DropDoubleOnStack(double Const) = 0;
    virtual void Rendor_DropConstCharOnStack(const char* Const) = 0;
    virtual void Rendor_DropBoolOnStack(int Const) = 0;
    virtual void Rendor_InitModule(RendorMethod *MethodList) = 0;
};


#endif // RENDOR_API_H
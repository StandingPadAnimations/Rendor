#ifndef RENDOR_API_H
#define RENDOR_API_H

#if __cplusplus 
#define EXTERNAL extern "C"
#define RENDOR_INIT extern "C" void
#else
#define EXTERNAL
#define RENDOR_INIT void
#endif

EXTERNAL struct RendorMethod
{
    const char* Name;
    size_t ArgCount;
    void (*RendorFunc)(void);
};

enum TypeEnum
{
    INT64,
    DOUBLE,
    STRING,
    BOOL
};

EXTERNAL int64_t Rendor_GrabInt64FromStack();
EXTERNAL double Rendor_GrabDoubleFromStack();
EXTERNAL const char* Rendor_GrabConstCharFromStack();
EXTERNAL const char* Rendor_GrabConstFromStackAsConstChar();
EXTERNAL TypeEnum Rendor_GrabObjTypeFromStack();
EXTERNAL int Rendor_GrabBoolFromStack();
EXTERNAL void Rendor_DropInt64OnStack(int64_t Const);
EXTERNAL void Rendor_DropDoubleOnStack(double Const);
EXTERNAL void Rendor_DropConstCharOnStack(const char* Const);
EXTERNAL void Rendor_DropBoolOnStack(int Const);
EXTERNAL void Rendor_InitModule(RendorMethod MethodList[]);

#endif // RENDOR_API_H
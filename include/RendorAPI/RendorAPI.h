#ifndef RENDOR_API_H
#define RENDOR_API_H

#if __cplusplus 
#define EXTERNAL extern "C"
#define RENDOR_INIT extern "C" void
#else
#define EXTERNAL
#define RENDOR_INIT void
#endif

#include "RendorAPI/RendorModuleDef.h"
enum TypeEnum
{
    C_INT64,
    C_DOUBLE,
    C_STRING,
    C_BOOL
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
EXTERNAL void Rendor_InitModule(RendorMethod *MethodList, size_t size);

#endif // RENDOR_API_H
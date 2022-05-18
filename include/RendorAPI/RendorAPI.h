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
    int64_t     (*Rendor_GrabInt64FromStack)(void);
    double      (*Rendor_GrabDoubleFromStack)(void);
    const char* (*Rendor_GrabConstCharFromStack)(void);
    const char* (*Rendor_GrabConstFromStackAsConstChar)(void);
    TypeEnum    (*Rendor_GrabObjTypeFromStack)(void);
    int         (*Rendor_GrabBoolFromStack)(void);

    void        (*Rendor_DropInt64OnStack)(int64_t);
    void        (*Rendor_DropDoubleOnStack)(double);
    void        (*Rendor_DropConstCharOnStack)(const char*);
    void        (*Rendor_DropBoolOnStack)(int);
    
    void        (*Rendor_InitModule)(const RendorMethod*);
    int         (*Rendor_ConstEval)(void);
};


#endif // RENDOR_API_H
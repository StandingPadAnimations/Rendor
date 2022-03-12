#ifndef RENDOR_MODULE_DEF_H
#define RENDOR_MODULE_DEF_H
#include <stdint.h>

#if __cplusplus 
#define EXTERNAL extern "C"
#else
#define EXTERNAL
#endif

EXTERNAL struct RendorMethod
{
    const char* Name;
    size_t ArgCount;
    void (*RendorFunc)(void);
};

#endif // RENDOR_MODULE_DEF_H
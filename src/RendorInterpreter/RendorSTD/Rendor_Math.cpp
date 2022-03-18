#include <stdint.h>
#include "RendorSTD/Math.hpp"

void RENDOR_SUM_FUNCTION()
{
    int64_t IntArg = Rendor_GrabInt64FromStack();
    IntArg = IntArg*(IntArg - 1) / 2;
    Rendor_DropInt64OnStack(IntArg);
}

static RendorMethod Rendor_MATH_InitList[] = {
    {"sum", 1, RENDOR_SUM_FUNCTION},
    {NULL, 0, NULL}
};

RENDOR_INIT RendorVM_Module_IO()
{
    RendorVM_InitModule(Rendor_MATH_InitList);
}

RENDOR_INIT RendorC_Module_IO()
{
    RendorC_InitModule(Rendor_MATH_InitList);
}

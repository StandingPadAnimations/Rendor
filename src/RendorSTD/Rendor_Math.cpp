#include <cstddef>
#include <stdint.h>
#include "RendorAPI/RendorAPI.h"

// sum is const evaluated
void RENDOR_SUM_FUNCTION(RendorState* R)
{
    int64_t IntArg = R->Rendor_GrabInt64FromStack();
    IntArg = IntArg*(IntArg - 1) / 2;
    R->Rendor_DropInt64OnStack(IntArg);
}

static RendorMethod Rendor_MATH_InitList[] = {
    {"sum", 1, RENDOR_SUM_FUNCTION},
    {NULL, 0, NULL}
};

RENDOR_INIT RendorVM_Module_IO(RendorState* R)
{
    R->Rendor_InitModule(Rendor_MATH_InitList);
}
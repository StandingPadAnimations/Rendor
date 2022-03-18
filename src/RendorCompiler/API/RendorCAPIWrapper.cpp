#include "RendorAPI/RendorAPI.h"
#include "RendorEngine.hpp"

EXTERNAL void RendorC_InitModule(RendorMethod *MethodList)
{
    Parser::InitModule(MethodList);
}
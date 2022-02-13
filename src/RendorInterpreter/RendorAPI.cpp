#include "RendorInterpreter/RendorInternalAPI.hpp"
#include "RendorInterpreter/RendorAPI.hpp"

/* ------------------------------ external API ------------------------------ */
int64_t GrabInt64FromArgs_API()
{
    return Interpreter::GrabInt64FromArgs();
}
double GrabDoubleFromArgs_API()
{
    return Interpreter::GrabDoubleFromArgs();
}
const char* GrabStringFromArgs_API()
{
    return Interpreter::GrabStringFromArgs();
}
bool GrabBoolFromArgs_API()
{
    return Interpreter::GrabBoolFromArgs();
}

/* ------------------------------ Internal API ------------------------------ */
int64_t GrabInt64FromConsts_API()
{
    return Interpreter::GrabInt64FromConsts();
}
double GrabDoubleFromConsts_API()
{
    return Interpreter::GrabDoubleFromConsts();
}
const char* GrabStringFromConsts_API()
{
    return Interpreter::GrabStringFromConsts();
}
bool GrabBoolFromConsts_API()
{
    return Interpreter::GrabBoolFromConsts();
}
VariableType GrabPointedConstType_API()
{
    return Interpreter::GrabPointedType();
}
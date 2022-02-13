#ifndef RENDOR_API_HPP
#define RENDOR_API_HPP
#include "RendorInterpreter/Interpreter.hpp"

/* ----------------------- Argument related functions ----------------------- */
extern "C" int64_t GrabInt64FromArgs_API();
extern "C" double GrabDoubleFromArgs_API();
extern "C" const char* GrabStringFromArgs_API();
extern "C" bool GrabBoolFromArgs_API();

/* -------- Constant related functions. Only to be used by internals -------- */
extern "C" int64_t GrabInt64FromConsts_API();
extern "C" double GrabDoubleFromConsts_API();
extern "C" const char* GrabStringFromConsts_API();
extern "C" bool GrabBoolFromConsts_API();
extern "C" VariableType GrabPointedConstType_API();
#endif // RENDOR_API_HPP
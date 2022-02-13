#ifndef RENDOR_INTERNAL_API_HPP
#define RENDOR_INTERNAL_API_HPP
#include "RendorInterpreter/Interpreter.hpp"

/* -------- Constant related functions. Only to be used by internals -------- */
extern "C" int64_t GrabInt64FromConsts_API();
extern "C" double GrabDoubleFromConsts_API();
extern "C" const char* GrabStringFromConsts_API();
extern "C" bool GrabBoolFromConsts_API();
extern "C" VariableType GrabPointedConstType_API();

#endif // RENDOR_INTERNAL_API_HPP
#ifndef RENDOR_MODULES_HPP
#define RENDOR_MODULES_HPP

#include <vector>

#include "RendorInterpreter/Objects/Stackframe.hpp"
#include "RendorInterpreter/Objects/Function.hpp"

struct Module
{
    StackFrame GlobalVars;
    std::vector<Function> Functions;
};

#endif // RENDOR_MODULES_HPP
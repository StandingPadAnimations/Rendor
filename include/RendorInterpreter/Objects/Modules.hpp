#ifndef RENDOR_MODULES_HPP
#define RENDOR_MODULES_HPP

#include <variant>
#include <vector>

#include "RendorInterpreter/Objects/Stackframe.hpp"
#include "RendorInterpreter/Objects/ConstPool.hpp"
#include "RendorInterpreter/Objects/Operation.hpp"

struct Module
{
    StackFrame GlobalVars;
    ConstPool Pool;
    std::vector<Operation> Program;
};

#endif // RENDOR_MODULES_HPP
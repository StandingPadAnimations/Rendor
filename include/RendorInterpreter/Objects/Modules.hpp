#ifndef RENDOR_MODULES_HPP
#define RENDOR_MODULES_HPP

#include <variant>
#include <vector>

#include "RendorInterpreter/Objects/Stackframe.hpp"
#include "RendorInterpreter/Objects/Function.hpp"
#include "RendorInterpreter/Objects/ConstPool.hpp"

struct Module
{
    StackFrame GlobalVars;
    ConstPool Pool;
    std::vector<std::variant<Function, Function*>> Functions;
    Function Main;
};

#endif // RENDOR_MODULES_HPP
#ifndef RENDOR_MODULE_HPP
#define RENDOR_MODULE_HPP

#include <string_view>

#include "RendorInterpreter/Objects/ConstPool.hpp"
#include "RendorInterpreter/Objects/Operation.hpp"

struct Module
{
    std::string_view Name;
    std::vector<Operation> Operations;
    ConstPool Pool;
};

#endif // RENDOR_MODULE_HPP
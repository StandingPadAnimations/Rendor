#ifndef RENDOR_FUNCTION_HPP
#define RENDOR_FUNCTION_HPP
#include <vector>

#include "RendorInterpreter/Objects/Operation.hpp"
struct Function 
{
    std::vector<Operation> FunctionOperations;
};

#endif // RENDOR_FUNCTION_HPP
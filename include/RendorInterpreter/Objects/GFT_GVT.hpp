#ifndef RENDOR_GFT_GVT_HPP
#define RENDOR_GFT_GVT_HPP

#include <unordered_map>

#include "RendorInterpreter/Objects/Stackframe.hpp"
#include "RendorInterpreter/Objects/Function.hpp"
#include "UnorderedMapLookUp.hpp"

struct GlobalFunctionTable
{
    std::unordered_map<std::string, Function, string_hash, std::equal_to<>> Functions;
};

struct GlobalVariableTable
{
    std::unordered_map<std::string, Constant*, string_hash, std::equal_to<>> Variables;
};
#endif // RENDOR_GFT_GVT_HPP
#ifndef RENDOR_API_FUNCTION_TABLE_HPP
#define RENDOR_API_FUNCTION_TABLE_HPP
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>
#include <map>
#include <set>
#include <functional>

#include "RendorAPI/RendorAPI.h"

struct FunctionTable
{
    inline static std::set<std::string> Functions;
    inline static std::map<std::string, size_t> FunctionsArgCount;
};

void Init(const RendorMethod* MethodList);

#endif // RENDOR_API_FUNCTION_TABLE_HPP
#ifndef RENDOR_API_FUNcTION_TABLE_HPP
#define RENDOR_API_FUNcTION_TABLE_HPP
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>

#include "RendorAPI/RendorAPI.h"

struct FunctionTable
{
    inline static std::map<std::string, std::function<void(RendorState*)>> Functions;
    inline static std::map<std::string, size_t> FunctionsArgCount;
};

void Init(const RendorMethod* MethodList);

#endif // RENDOR_API_FUNcTION_TABLE_HPP
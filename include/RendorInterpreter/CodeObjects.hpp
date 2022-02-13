#ifndef RENDOR_CODE_OBJECTS_HPP
#define RENDOR_CODE_OBJECTS_HPP
#include <vector>
#include <memory>
#include <string_view>

using std::vector;
using std::unique_ptr;
using std::string_view;

#include "RendorInterpreter/RendorTypes.hpp"
#include "RendorInterpreter/RendorJit.hpp"

struct RendorCodeObject
{
    RendorCodeObject() = default;
    virtual ~RendorCodeObject(){};
};

struct Function : RendorCodeObject
{
    std::vector<VariableType> TypesForPastCall;
    std::vector<std::string_view> ByteCode;
    RendorFunc* JITFunc;
};

struct Script : RendorCodeObject
{
    std::vector<std::unique_ptr<Function>> Functions;
};

#endif // RENDOR_CODE_OBJECTS_HPP
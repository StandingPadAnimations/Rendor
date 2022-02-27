#ifndef RENDOR_CODE_OBJECTS_HPP
#define RENDOR_CODE_OBJECTS_HPP
#include <vector>
#include <memory>
#include <string_view>

using std::vector;
using std::unique_ptr;
using std::string_view;

#include "RendorInterpreter/Resources/RendorTypes.hpp"
#include "RendorInterpreter/JIT/RendorJit.hpp"
#include "RendorInterpreter/Resources/RendorDefinitions.hpp"

struct RendorCodeObject
{
    RendorCodeObject() = default;
    virtual ~RendorCodeObject(){};
};

struct Function : RendorCodeObject
{
    std::vector<std::string_view> ByteCode;
    std::vector<std::string> IR{10};
    RendorFunc* JITFunc;
};

struct Script : RendorCodeObject
{
    std::vector<std::unique_ptr<Function>> Functions;
};

#endif // RENDOR_CODE_OBJECTS_HPP
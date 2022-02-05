#ifndef RENDOR_CODE_OBJECTS_HPP
#define RENDOR_CODE_OBJECTS_HPP
#include <vector>
#include <memory>
#include <string_view>

struct RendorCodeObject
{
    RendorCodeObject() = default;
    virtual ~RendorCodeObject(){};
};

struct Function : RendorCodeObject
{
    std::vector<std::string_view> ByteCode;
};

struct Script : RendorCodeObject
{
    std::vector<std::unique_ptr<Function>> Functions;
};

#endif // RENDOR_CODE_OBJECTS_HPP
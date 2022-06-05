#ifndef IR_CONTEXT_HPP
#define IR_CONTEXT_HPP
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "RendorCompiler/BackEnd/IR/IR_Obj.hpp"

using IR_Object = std::unique_ptr<IR::Obj>;
using IR_Const = std::unique_ptr<IR::data::RendorConst>;
struct IR_Context
{
    std::map<std::string, std::uint16_t, std::less<>> Global_var_to_reg;
    std::vector<std::map<std::string, std::uint16_t, std::less<>>> Var_to_Reg;
    std::map<std::string, std::uint16_t, std::less<>> Func_to_Reg;
    std::vector<IR_Const> ConstPool;
    std::vector<IR_Object> IR;
};

namespace gen
{
    namespace data
    {
        IR_Object CreateInt(std::int64_t val);
        IR_Object CreateDouble(double val);
        IR_Object CreateString(std::string val);
        IR_Object CreateBool(bool val);
        std::size_t AddtoPool(IR_Object Object);
    }
    namespace var
    {
        IR_Object MovVar(std::string Dst, std::size_t Src);
        IR_Object CpyVar(std::string Dst, std::size_t Src);
        IR_Object refVar(std::string Dst, std::size_t Src);
    }
    namespace func 
    {
        IR_Object DefineFunc(std::string Name);
    }
    namespace mem 
    {
        IR_Object Alloc(std::size_t Size);
        IR_Object Free();
    }
}
#endif // IR_CONTEXT_HPP
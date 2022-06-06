#ifndef IR_CONTEXT_HPP
#define IR_CONTEXT_HPP
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "RendorCompiler/BackEnd/IR/IR_Obj.hpp"

using IR_Object = std::unique_ptr<IR::Obj>;
using IR_vec    = std::vector<IR_Object>;
using IR_Const  = std::unique_ptr<IR::data::RendorConst>;
using IR_ptr    = IR::Obj*;
using NameToReg = std::map<std::string, IR_ptr, std::less<>>;
using Scope     = std::pair<NameToReg, IR::reg_mod::Frame*>;
struct IR_Context
{
    NameToReg             Global_var_to_reg;
    std::vector<Scope>    Var_to_Reg;
    NameToReg             Func_to_Reg;
    std::vector<IR_Const> ConstPool;
    IR_vec  IR;
};

namespace gen
{
    /*
    *   Vec: represents the current scope
    *   Dst: The destination register
    */
    namespace data
    {
        IR_ptr CreateInt(std::int64_t val);
        IR_ptr CreateDouble(double val);
        IR_ptr CreateString(std::string val);
        IR_ptr CreateBool(bool val);
        IR_ptr CreateArray();
    }
    namespace var
    {
        IR_ptr Name_to_Ptr(std::string_view Name);  // ! Returns nullptr if the name doesn't exist
        IR_ptr Add_var    (std::string Name);       // ! Use this if you want to add a new variable
        void   MovVar(IR_ptr Dst, IR_ptr Src, IR_vec* Vec = nullptr);
        void   CpyVar(IR_ptr Dst, IR_ptr Src, IR_vec* Vec = nullptr);
        void   refVar(IR_ptr Dst, IR_ptr Src, IR_vec* Vec = nullptr);
    }
    namespace func 
    {
        IR_ptr Name_to_Ptr(std::string_view Name);  // ! Returns nullptr if the name doesn't exist
        IR_ptr DefineFunc(std::string Name);        // ! Use this if you want to add a new function
        void   CallFunction(IR_ptr Func, std::vector<IR_ptr> Args, IR_ptr Ret = nullptr);
    }
    namespace mem 
    {
        IR_ptr CreateTempRegister(); // ! Use this to create a temporary register
        void   Alloc(IR_vec* Vec = nullptr);
        void   Free (IR_vec* Vec = nullptr);
    }
    namespace math 
    {
        void   Add(IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Sub(IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Mul(IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Div(IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Pow(IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Icr(IR_ptr Dst, IR_vec* Vec = nullptr);
        void   Dcr(IR_ptr Dst, IR_vec* Vec = nullptr);
    }
    namespace logic 
    {
        void   Eq     (IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Not_Eq (IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Gr     (IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Less   (IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Gr_Eq  (IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
        void   Less_Eq(IR_ptr Dst, IR_ptr Val1, IR_ptr Val2, IR_vec* Vec = nullptr);
    }
}
#endif // IR_CONTEXT_HPP
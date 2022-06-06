#ifndef IR_OBJ_HPP
#define IR_OBJ_HPP
#include <cstdint>
#include <variant>
#include <string>
#include <vector>
#include <memory>

#include "RendorCompiler/BackEnd/IR/IRDefinitions.hpp"
#include "RendorCompiler/BackEnd/IR/IRRegLayout.hpp"

namespace IR
{
    struct Obj {IR_def Op;};

    namespace reg_mod 
    {
        struct Frame : Obj {std::size_t Size;};
        struct Mov   : Obj {Register Regs;};
        struct Mov_n : Obj {Register Regs;};
        struct Cpy   : Obj {Register Regs;};
        struct Ref   : Obj {Register Regs;};
    }

    namespace stk_mod 
    {
        struct Mov   : Obj {Reg64 Regs;};
        struct Mov_n : Obj {Reg64 Regs;};
        struct Cpy   : Obj {Reg64 Regs;};
        struct Ref   : Obj {Reg64 Regs;};
    }

    namespace var
    {
        struct AssignVariable : Obj
        {
            Register Regs;
            std::string VarName;
        };
    }

    namespace func
    {
        struct FunctionCall : Obj 
        {
            FunctionCallReg Registers;
            std::string Name;
        };
        struct FunctionDefinition : Obj 
        {
            std::uint32_t FunctionReg;
            std::string Name;
            std::vector<std::unique_ptr<Obj>> Definition;
        };
    }
    
    namespace math
    {
        struct Operation : Obj {Register Regs;};
        struct Increment : Obj {Reg64 Regs;};
        struct Decrement : Obj {Reg64 Regs;};
    }

    namespace data 
    {
        struct RendorConst  : Obj         {std::size_t Loc;};
        struct RendorInt    : RendorConst {std::int64_t Val;};
        struct RendorDouble : RendorConst {double Val;};
        struct RendorString : RendorConst {std::string Val;};
        struct RendorBool   : RendorConst {bool Val;};
        struct RendorArray  : RendorConst {std::vector<RendorConst> Val;};
        struct RendorRef    : RendorConst {Obj* Val;};
        struct RendorNone   : RendorConst {};
    }

    namespace logic 
    {
        struct RendorLogic : Obj {Register Reg;};
    }
}
#endif // IR_OBJ_HPP
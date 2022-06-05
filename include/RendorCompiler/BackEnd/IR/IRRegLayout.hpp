#ifndef IR_REG_LAYOUT_HPP
#define IR_REG_LAYOUT_HPP
#include <cstdint>

struct Register 
{
    std::uint32_t Reg1, Reg2, Reg3;
    std::uint8_t Location1, Location2, Location3;
};

struct Reg64 
{
    std::uint64_t Reg;
    std::uint8_t Location;
};

struct FunctionCallReg
{
    std::uint16_t Ret;
    std::uint32_t Func;
    std::uint8_t Ret_o_not;
    std::uint8_t Ret_loc;
};


#endif // IR_REG_LAYOUT_HPP
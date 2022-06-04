#ifndef RENDOR_OPERATION_HPP
#define RENDOR_OPERATION_HPP

#include "Definitions.hpp"
#include <cstdint>

struct Operation 
{
    ByteCodeEnum Operation;

    /* ------------------------ For typical instructions ------------------------ */
    std::uint16_t Reg1 = 0;
    std::uint16_t Reg2 = 0;
    std::uint16_t Reg3 = 0;
    std::uint8_t  Reg1_type = 0;
    std::uint8_t  Reg2_type = 0;
    std::uint8_t  Reg3_type = 0;

    /* ----------------------------------- JMP ---------------------------------- */
    std::uint64_t JMP_amount = 0;
    std::uint8_t  For_o_back = 0;

    /* ----------------------------- Function Calls ----------------------------- */
    std::uint16_t RetReg = 0;
    std::uint32_t FunctionReg = 0;
    std::uint8_t  Ret_or_not = 0;
    std::uint8_t  Ret_type = 0;
};

#endif // RENDOR_OPERATION_HPP
#ifndef RENDOR_OPERATION_HPP
#define RENDOR_OPERATION_HPP

#include "Definitions.hpp"
#include <cstdint>

struct Operation 
{
    ByteCodeEnum Operation;

    /* ------------------------ For typical instructions ------------------------ */
    std::uint16_t Reg1_16     = 0;
    std::uint16_t Reg2_16     = 0;
    std::uint16_t Reg3_16     = 0;
    std::uint8_t  Reg1_type_8      = 0;
    std::uint8_t  Reg2_type_8      = 0;
    std::uint8_t  Reg3_type_8      = 0;

    /* ---------------------------- mov, cpy, and ref --------------------------- */
    std::uint16_t Reg1_ref    = 0;
    std::uint16_t Reg2_ref    = 0;
    /* ----------------------------------- JMP ---------------------------------- */
    std::uint64_t Reg_64 = 0;
    std::uint8_t  Reg_64_type_8 = 0;
};

#endif // RENDOR_OPERATION_HPP
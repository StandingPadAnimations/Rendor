#ifndef RENDOR_OPERATION_HPP
#define RENDOR_OPERATION_HPP

#include "Definitions.hpp"

struct Operation 
{
    ByteCodeEnum Operation;

    bool UseReg2 = false;

    std::size_t Reg1;
    std::size_t Reg2;
    RendorVariant Value;
};

#endif // RENDOR_OPERATION_HPP
#ifndef RENDOR_CONST_POOL_HPP
#define RENDOR_CONST_POOL_HPP

#include <vector>
#include "RendorInterpreter/Objects/Constant.hpp"
struct ConstPool 
{
    std::vector<Constant> Pool;
    Constant* operator[](std::size_t index)
    {
        return &Pool[index];
    }
};

#endif // RENDOR_CONST_POOL_HPP
#ifndef RENDOR_CONST_POOL_HPP
#define RENDOR_CONST_POOL_HPP

#include <vector>
#include "RendorInterpreter/Objects/Constant.hpp"
struct ConstPool 
{
    std::vector<Constant> Pool;
    ConstPool() = default;
    explicit ConstPool(std::uint32_t t_size) 
    {
        Pool.resize(t_size);
    }
    Constant* operator[](std::size_t index) noexcept
    {
        return &Pool[index];
    }
    void insert(Constant Value)
    {
        Pool.push_back(std::move(Value));
    }
};

#endif // RENDOR_CONST_POOL_HPP
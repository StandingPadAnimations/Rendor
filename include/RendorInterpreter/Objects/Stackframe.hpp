#ifndef RENDOR_STACK_FRAME_HPP
#define RENDOR_STACK_FRAME_HPP

#include <cstddef>
#include <variant>
#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <iterator>

#include "RendorInterpreter/Objects/Constant.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"

struct StackFrame 
{
    StackFrame* LastStackFrame = nullptr;
    std::vector<Constant> Registers;
    Constant* Ret = nullptr;

    StackFrame() = default;
    explicit StackFrame(std::size_t size)
    {
        Registers.reserve(size + 2);
        for (std::size_t i = 0; i < size; ++i)
        {
            Registers[i] = std::move(Constant{RendorConst{false}, ConstType::NONE});
        }
    }
    
    Constant& operator[](std::size_t index)
    {
        return Registers[index];
    }

    void insert(std::size_t index, Constant Object)
    {
        Registers[index] = std::move(Object);
    }
};

#endif // RENDOR_STACK_FRAME_HPP
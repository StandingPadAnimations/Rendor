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
    Registers Registers;
    Constant* Ret = nullptr;

    StackFrame() = default;
    explicit StackFrame(std::size_t size)
    {
        Registers.resize(size);
    }
    
    Constant& operator[](std::size_t index) noexcept
    {
        return Registers[index];
    }

    void insert(std::size_t index, Constant Object) noexcept
    {
        Registers[index] = std::move(Object);
    }
};

#endif // RENDOR_STACK_FRAME_HPP
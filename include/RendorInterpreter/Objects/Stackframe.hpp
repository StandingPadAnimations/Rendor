#ifndef RENDOR_STACK_FRAME_HPP
#define RENDOR_STACK_FRAME_HPP

#include <cstddef>
#include <vector>
#include <cstdint>

#include "RendorInterpreter/Objects/Constant.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"

struct StackFrame 
{
    StackFrame* LastStackFrame = nullptr;
    std::vector<Constant> Registers;
    std::uint32_t ip = 0;

    StackFrame() = default;
    explicit StackFrame(std::size_t size)
    {
        Registers.reserve(size);
    }

    Constant* operator[](std::size_t index)
    {
        if (Registers.size() < index)
        {
            Registers.push_back(Constant{});
            return &Registers[index];
        }
        return &Registers[index];
    }

    void push_back(Constant Object)
    {
        Registers.push_back(std::move(Object));
    }
};

#endif // RENDOR_STACK_FRAME_HPP
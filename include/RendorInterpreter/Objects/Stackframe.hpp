#ifndef RENDOR_STACK_FRAME_HPP
#define RENDOR_STACK_FRAME_HPP

#include <cstddef>
#include <variant>
#include <vector>
#include <cstdint>

#include "RendorInterpreter/Objects/Constant.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"

struct StackFrame 
{
    StackFrame* LastStackFrame = nullptr;
    std::vector<std::variant<Constant, Constant*>> Registers;
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
            throw error::RendorException("Segmentation Fault: Missing Register");
        }
        
        switch (Registers[index].index())
        {
            case 0:
            {
                return std::get_if<Constant>(&Registers[index]);
            }
            case 1:
            {
                return *std::get_if<Constant*>(&Registers[index]);
            }
        }
        return nullptr;
    }

    void push_back(Constant Object)
    {
        Registers.push_back(std::move(Object));
    }

    void push_back(Constant* Object)
    {
        Registers.push_back(std::move(Object));
    }
};

#endif // RENDOR_STACK_FRAME_HPP
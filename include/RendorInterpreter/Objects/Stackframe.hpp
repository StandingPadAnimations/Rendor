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
    std::vector<std::variant<Constant, Constant*>> Registers;

    Constant  ConditionRegister = Constant{true};
    Constant* RetRegister       = nullptr;
    
    StackFrame() = default;
    explicit StackFrame(std::size_t size)
    {
        Registers.reserve(size);
        std::fill_n(std::back_inserter(Registers), size, nullptr);
    }
    
    Constant* operator[](std::size_t index)
    {
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

    void insert(std::size_t index, Constant Object)
    {
        Registers[index] = std::move(Object);
    }

    void insert(std::size_t index, Constant* Object)
    {
        Registers[index] = std::move(Object);
    }
};

#endif // RENDOR_STACK_FRAME_HPP
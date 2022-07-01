#ifndef IR_OBJECTS_HPP
#define IR_OBJECTS_HPP
#include <cstdint>

#include "RendorCompiler/BackEnd/IR/IRDefinitions.hpp"

#include <fmt/core.h>

namespace RendorIR
{
    inline namespace v1_0_0
    {
        struct Value
        {
            IR_def ValueType{};
            virtual void CodeGen() {};
            virtual void PrintAST() = 0;

            inline static std::size_t Depth = 0;
            void PrintDepth(std::size_t offset = 0) const noexcept
            {
                for (std::size_t i = 0; i < Depth + offset; ++i)
                    fmt::print("----");
            }

            explicit Value(const IR_def Val) noexcept : ValueType(Val) {}
            virtual ~Value();
        };

        namespace IR_Consts
        {
            struct Constant : Value 
            {
                explicit Constant(const IR_Type TypeVal) noexcept : Value(IR_def::CONSTANT), Constant_Type(TypeVal){}
                IR_Type Constant_Type{};
            };
        }

        struct Assignment : Value 
        {
            Assignment() noexcept : Value(IR_def::ASSIGN_VARIABLE){}
            std::int16_t VarAddress{}; // Location of the variable
            IR_Consts::Constant* AssignVal = nullptr;
        };
    }
}

#endif // IR_OBJECTS_HPP
#ifndef IR_CONTEXT_HPP
#define IR_CONTEXT_HPP
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "RendorCompiler/BackEnd/IR/IRDefinitions.hpp"
#include "RendorCompiler/BackEnd/IR/IRObject.hpp"

namespace RendorIR
{
    inline namespace v1_0_0
    {
        using Str_to_Int = std::map<std::string, std::uint16_t>;
        using Str_to_Type = std::map<std::string, IR_Type>;
        struct VarFrame 
        {
            Str_to_Int Regs;
            std::uint16_t RegCount = 0;

            void add_var(const std::string& Name)
            {
                Regs[Name] = RegCount;
                ++RegCount;
            }
        };
        struct IR_Context
        {
            std::vector<VarFrame> Variables;
            std::vector<std::unique_ptr<RendorIR::IR_Consts::Constant>> Constants;
            Str_to_Int Addreses;
        };
    }
}

#endif // IR_CONTEXT_HPP
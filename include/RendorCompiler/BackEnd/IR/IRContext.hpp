#ifndef IR_CONTEXT_HPP
#define IR_CONTEXT_HPP
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "RendorCompiler/BackEnd/IR/IRDefinitions.hpp"


namespace RendorIR
{
    inline namespace v1_0_0
    {
        using Str_to_Int = std::map<std::string, std::int16_t>;
        using Str_to_Type = std::map<std::string, IR_Type>;
        struct VarFrame 
        {
            Str_to_Int Regs; 
            Str_to_Type RegTypes;

            std::uint16_t RegCount = 0;

            void add_var(const std::string& Name, IR_Type Type)
            {
                Regs[Name] = RegCount;
                RegTypes[Name] = Type;
                ++RegCount;
            }
        };
        struct IR_Context
        {
            std::vector<VarFrame> Variables;
            Str_to_Int Addreses;

            template <typename AssignmentType>
            void CreateVariable(const std::string& Name, IR_Type Type, AssignmentType Val);
            template <typename AssignmentType>
            void CreateGlobalVariable(const std::string& Name, IR_Type Type, AssignmentType Val);

            void DefineFunction(const std::string& FunctionName);
            void CallFunction(const std::string& FunctionName);
        };

    }
}

// #include "RendorCompiler/BackEnd/IR/Variables.ipp"
#endif // IR_CONTEXT_HPP
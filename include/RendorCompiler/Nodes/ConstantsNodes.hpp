#ifndef RENDOR_CONSTANTS_NODES_HPP
#define RENDOR_CONSTANTS_NODES_HPP
#include <algorithm>
#include <string>
#include <cstdint>
#include <memory>

#include "RendorCompiler/Nodes/BaseNode.hpp"
namespace nodes
{
    struct FunctionCall;
    struct Constant : Node 
    {
        Constant() = default;
        IR_Type Type_ir{};
        explicit Constant(NodeType input_Type, IR_Type input_Type_ir) : Node(input_Type), Type_ir(input_Type_ir){}
        virtual void CodeGen() override {}
        void PrintAST() override {};
    };
    struct Int : Constant
    {
        std::int64_t Val{};
        Int() : Constant(NodeType::INT, IR_Type::INT){}
        void PrintAST() final 
        {
            fmt::print("Int: {}\n", Val);
        }
    };
    struct Double : Constant 
    {
        double Val{};
        Double() : Constant(NodeType::DOUBLE, IR_Type::DOUBLE){}
        void PrintAST() final 
        {
            fmt::print("Double: {}\n", Val);
        }
    };
    struct String : Constant
    {
        std::string Val{};
        String() : Constant(NodeType::STRING, IR_Type::STRING){}
        void PrintAST() final 
        {
            fmt::print("String: {}\n", Val);
        }
    };
    struct Bool : Constant 
    {
        bool Val{};
        Bool() : Constant(NodeType::BOOL, IR_Type::BOOL){}
        void PrintAST() final 
        {
            fmt::print("Bool: {}\n", Val);
        }
    };
    struct Void : Constant
    {
        Void() : Constant(NodeType::VOID, IR_Type::VOID){}
        void PrintAST() final 
        {
            fmt::print("VOID\n");
        }
    };
    struct Reference : Constant
    {
        std::string Val{};
        Reference() : Constant(NodeType::VARIABLE_REFERENCE, IR_Type::REFERENCE){}
        void PrintAST() final 
        {
            fmt::print("Reference: {}\n", Val);
        }
    };
    struct FunctionCall : Constant 
    {
        std::string Name{};
        std::vector<std::unique_ptr<Constant>> Args;
        FunctionCall() : Constant(NodeType::FUNCTION_CALL, IR_Type::RET){}
        void PrintAST() final 
        {
            fmt::print("Function Call: {} (", Name);
            for (const auto& Arg : Args)
            {
                if (!Args.empty())
                    fmt::print("\n");

                PrintDepth(1);
                Arg->PrintAST();
            }
            if (!Args.empty())
            {
                PrintDepth();
            }
            fmt::print(")\n");
        }
        void CodeGen() override
        {
            
        };
    };
}

#endif // RENDOR_CONSTANTS_NODES_HPP
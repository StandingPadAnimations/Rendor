#ifndef RENDOR_BASE_NODE_HPP
#define RENDOR_BASE_NODE_HPP
#include "RendorCompiler/BackEnd/IR/IRContext.hpp"
#include "RendorEngine.hpp"
#include <fmt/core.h>
namespace nodes
{
    enum class NodeType
    {
        BASE_NODE,
        BODY,

        VARIABLE_ASSIGNMENT,
        VARIABLE_REFERENCE,

        FUNCTION_CALL,
        FUNCTION_DEFINITION,
        MAIN,

        TYPE_HINT,
        INT,
        DOUBLE,
        STRING,
        BOOL,
        VOID,
        CALL_RET,

        BIOP,
    };
    
    struct Node 
    {
        NodeType Type = NodeType::BASE_NODE;
        inline static auto& Context_IR =  RendorEngineCompiler::Context_IR;
        
        Node() = default;
        explicit Node(NodeType input_Type) : Type(input_Type){}

        virtual void CodeGen() {};
        virtual void PrintAST() = 0;

        inline static std::size_t Depth = 0;
        void PrintDepth(std::size_t offset = 0)
        {
            for (std::size_t i = 0; i < Depth+offset; ++i)
                fmt::print("----");
        }
    };
}

#endif // RENDOR_BASE_NODE_HPP
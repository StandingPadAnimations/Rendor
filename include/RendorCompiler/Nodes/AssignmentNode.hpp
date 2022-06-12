#ifndef RENDOR_ASSIGNMENT_NODE
#define RENDOR_ASSIGNMENT_NODE
#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/ConstantsNodes.hpp"
namespace nodes 
{
    struct Assignment : Node 
    {
        std::string Name{};
        std::unique_ptr<Constant> Val;
        IR_Type ConstType{};
        Assignment() : Node(NodeType::VARIABLE_ASSIGNMENT){}

        void PrintAST() final 
        {
            fmt::print("Assignment: {} of type {}\n", Name, ConstType);
            PrintDepth(1);
            Val->PrintAST();
        }

        void CodeGen() final
        {
            
        }
    };
}
#endif // RENDOR_ASSIGNMENT_NODE
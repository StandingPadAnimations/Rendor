#ifndef IF_ELSE_NODE_HPP
#define IF_ELSE_NODE_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNodes.hpp"
#include "RendorCompiler/Nodes/LogicNodes.hpp"

struct IfElse : Node
{
    std::unique_ptr<Condition> Conditions;
    Body IfElseBody;
    std::unique_ptr<IfElse> ElseStatement;
    IfElse(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::IfElse;
    }
};

#endif // IF_ELSE_NODE_HPP
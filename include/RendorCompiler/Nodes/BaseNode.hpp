#ifndef RENDOR_BASE_NODE_HPP
#define RENDOR_BASE_NODE_HPP

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

        BIOP,
    };
    
    struct Node 
    {
        NodeType Type = NodeType::BASE_NODE;

        explicit Node(NodeType input_Type) : Type(input_Type) {}
    };
}

#endif // RENDOR_BASE_NODE_HPP
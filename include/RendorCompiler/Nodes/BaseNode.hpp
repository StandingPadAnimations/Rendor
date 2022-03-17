#ifndef BASE_NODE_HPP
#define BASE_NODE_HPP

#include <string>
#include <vector>
#include <memory>

enum class TempID{
    None,

    // Definitions
    ArithAssemble,
    IdentifierDefinition,
    VariableDefition,
    FowardDefinition,
    FowardArgsDefinition,
    FunctionDefiniton,
    FunctionArgumentsDefinition,

    // Function things
    FunctionCall,
    FunctionScope,

    // If-else statements 
    IfStatementDefinition,
    ElseDefinition,
    ConditionDefinition,
    IfElseScope
};

enum class NodeType{
    Body,
    Scope,
    Rdef,
    FowardEdef,
    AssignVariable,
    IfElse,
    Increment,
    Decrement,
    FunctionCall,
    Edef,
    Condition,
    BiOp,

    Any,
    Int, 
    Double,
    String,
    Bool,
    Reference,
    Arithmethic,
};

struct Node{
    NodeType Type;
    uint32_t LineNumber = 0;
    
    Node() = default;
    Node(uint32_t LineNumber) : LineNumber(LineNumber){}
    virtual ~Node(){};
};

typedef std::unique_ptr<Node> NodeObject;

#endif // BASE_NODE_HPP
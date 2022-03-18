#ifndef BASE_NODE_HPP
#define BASE_NODE_HPP

#include <string>
#include <vector>
#include <memory>

#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"

enum class TempID
{
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

enum class NodeType
{
    Body,
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

struct Node
{
    NodeType Type;
    ByteCodeType ConstType;
    uint32_t LineNumber = 0;
    
    RendorByteCodeGenerator NodeByteCodeGen;
    Node() = default;
    Node(uint32_t LineNumber) : LineNumber(LineNumber){}
    virtual ~Node(){};
    virtual void CodeGen() = 0;
};

typedef std::unique_ptr<Node> NodeObject;

#endif // BASE_NODE_HPP
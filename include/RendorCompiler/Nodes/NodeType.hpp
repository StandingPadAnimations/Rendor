#ifndef NODETYPE_HPP
#define NODETYPE_HPP

enum class NodeType
{
    Null,
    Body,
    Import,
    Export,
    FowardEdef,
    AssignVariable,
    IfElse,
    Increment,
    Decrement,
    FunctionCall,
    Edef,
    Attribute,
    Condition,
    BiOp,

    Any,
    Int64, 
    Double,
    String,
    Bool,
    Reference,
    Arithmethic,
};

#endif // NODETYPE_HPP
#ifndef NODETYPE_HPP
#define NODETYPE_HPP

enum class NodeType
{
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

    Null,
    Any,
    Int64, 
    Double,
    String,
    Bool,
    Reference,
    Arithmethic,
};

#endif // NODETYPE_HPP
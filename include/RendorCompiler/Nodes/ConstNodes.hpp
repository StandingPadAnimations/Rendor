#ifndef CONST_NODES_HPP
#define CONST_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct Int : Node
{
    std::string Value = "";
    
    Int(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Int;
    }
};

struct Double : Node
{
    std::string Value = "";
    
    Double(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Double;
    }
};

struct String : Node
{
    std::string Value = "";
    
    String(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::String;
    }
};

struct Bool : Node
{
    std::string Value = "";
    
    Bool(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Bool;
    }
};

struct Arithmethic : Node
{
    std::string Value = "";
    
    Arithmethic(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Arithmethic;
    }
};

struct Reference : Node
{
    std::string Value = "";

    Reference(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Reference;
    }
};

#endif // CONST_NODES_HPP
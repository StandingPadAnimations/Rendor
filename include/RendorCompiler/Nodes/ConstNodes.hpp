#ifndef CONST_NODES_HPP
#define CONST_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"

struct Int : Node
{
    std::string Value = "";
    
    Int(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Int;
        ConstType = ByteCodeType::INT64;
    }

    void CodeGen()
    {
        NodeByteCodeGen.CreateConst(ConstType, Value);
    }
};

struct Double : Node
{
    std::string Value = "";
    
    Double(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Double;
        ConstType = ByteCodeType::DOUBLE;
    }

    void CodeGen()
    {
        NodeByteCodeGen.CreateConst(ConstType, Value);
    }
};

struct String : Node
{
    std::string Value = "";
    
    String(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::String;
        ConstType = ByteCodeType::STRING;
    }

    void CodeGen()
    {
        NodeByteCodeGen.CreateConst(ConstType, Value);
    }
};

struct Bool : Node
{
    std::string Value = "";
    
    Bool(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Bool;
        ConstType = ByteCodeType::BOOL;
    }

    void CodeGen()
    {
        NodeByteCodeGen.CreateConst(ConstType, Value);
    }
};

struct Arithmethic : Node
{
    std::string Value = "";
    
    Arithmethic(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Arithmethic;
        ConstType = ByteCodeType::ARITH;
    }

    void CodeGen()
    {
        NodeByteCodeGen.CreateConst(ConstType, Value);
    }
};

struct Reference : Node
{
    std::string Value = "";

    Reference(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber), Value(ValueInput)
    {
        Type = NodeType::Reference;
        ConstType = ByteCodeType::REFERENCE;
    }

    void CodeGen()
    {
        NodeByteCodeGen.CreateConst(ConstType, Value);
    }
};

#endif // CONST_NODES_HPP
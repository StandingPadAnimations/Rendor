#ifndef BASE_NODE_HPP
#define BASE_NODE_HPP

#include <string>
#include <vector>
#include <memory>

#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"
#include "RendorCompiler/Nodes/NodeType.hpp"

struct Node
{
    NodeType Type;
    ByteCodeType ConstType;
    uint32_t LineNumber = 0;
    RendorByteCodeGenerator NodeByteCodeGen;
    Node() = default;
    
    //cppcheck-suppress uninitMemberVar
    explicit Node(uint32_t LineNumber) : LineNumber(LineNumber){}
    virtual ~Node(){};
    virtual void CodeGen() = 0;
};

typedef std::unique_ptr<Node> NodeObject;

#endif // BASE_NODE_HPP
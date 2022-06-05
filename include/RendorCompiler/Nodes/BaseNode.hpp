#ifndef BASE_NODE_HPP
#define BASE_NODE_HPP

#include <string>
#include <vector>
#include <memory>

#include "RendorCompiler/BackEnd/IR/IRContext.hpp"
#include "RendorCompiler/Nodes/NodeType.hpp"
#include "RendorEngine.hpp"

struct Node
{
    NodeType Type;
    IR_Type ConstType;
    uint32_t LineNumber = 0;

    #if DEBUG_NODES
    std::string ID;
    inline static size_t ID_Num = 0;
    #endif

    Node() = default;
    explicit Node(uint32_t LineNumber) : LineNumber(LineNumber)
    {
        #if DEBUG_NODES
        ++ID_Num;
        ID = fmt::format("RENDOR_NODE {}", ID_Num);
        #endif
    }
    virtual ~Node(){};
    virtual void CodeGen() = 0;
};

typedef std::unique_ptr<Node> NodeObject;

#endif // BASE_NODE_HPP
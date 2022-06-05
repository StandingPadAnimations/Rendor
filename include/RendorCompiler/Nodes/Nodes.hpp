#ifndef RENDOR_NODES_HPP
#define RENDOR_NODES_HPP

#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNodes.hpp"
#include "RendorCompiler/Nodes/ConstNodes.hpp"
#include "RendorCompiler/Nodes/FunctionNodes.hpp"
#include "RendorCompiler/Nodes/IfElseNode.hpp"
#include "RendorCompiler/Nodes/IncrDecrNodes.hpp"
#include "RendorCompiler/Nodes/LogicNodes.hpp"
#include "RendorCompiler/Nodes/VariableNode.hpp"
#include "RendorCompiler/Nodes/ImportNodes.hpp"
#include "RendorCompiler/Nodes/ExportNode.hpp"
#include "RendorCompiler/Nodes/NodeType.hpp"

struct Main
{
    Body Global;
    std::vector<NodeObject> *GlobalBody = &Global.ConnectedNodes;
};
#endif // RENDOR_NODES_HPP
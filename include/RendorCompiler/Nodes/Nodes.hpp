#include "RendorCompiler/Nodes/AssignmentNode.hpp"
#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNode.hpp"
#include "RendorCompiler/Nodes/ConstantsNodes.hpp"
#include "RendorCompiler/Nodes/FunctionNode.hpp"
#include "RendorCompiler/Nodes/ImportNodes.hpp"

using ConstPtr = std::unique_ptr<nodes::Constant>;
using ConstPtr = std::unique_ptr<nodes::Constant>;

struct Main
{
    nodes::ScriptBody GlobalScope{};
    void PrintTree()
    {
        GlobalScope.PrintAST();
    }
};

#ifndef RENDOR_FUNCTION_DECLARATION_HPP
#define RENDOR_FUNCTION_DECLARATION_HPP

#define FALLTHROUGH [[fallthrough]]
#include <array>
#include <vector>
#include <string_view>
#include <string>
#include <map>

#include "RendorCompiler/Nodes/NodeType.hpp"

struct DeclarationStorage
{
    std::vector<std::vector<NodeType>*> Declarations;
};

class FunctionDeclaration
{
    public:
        std::vector<NodeType>* GetFunction(const NodeType ReturnType, const std::vector<NodeType>& Types);
        void AddFunction(const NodeType ReturnType, std::vector<NodeType>& Types);

    private:
        const std::map<NodeType, size_t, std::less<>> TypeIndexTable 
        {
            {NodeType::Any,    0},
            {NodeType::Int64,  1},
            {NodeType::Double, 2},
            {NodeType::String, 3},
            {NodeType::Bool,   4},
        };

        // slots for each type and compatible arguments.
        // anything with an any argument is added to the end
        std::array<DeclarationStorage, 5> FunctionDeclarations;
};


#endif // RENDOR_OVERLOAD_CLASS
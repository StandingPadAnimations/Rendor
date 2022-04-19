#ifndef AST_INSPECTOR_HPP
#define AST_INSPECTOR_HPP

#include <vector>
#include <memory>

#include "RendorCompiler/Nodes/Nodes.hpp"

#define FALLTHROUGH [[fallthrough]]
typedef std::map<std::string_view, NodeType> VariableMap;
typedef std::vector<NodeType> FunctionArgsVector;

class ASTInspector
{
    public:
        static void InspectAST(const NodeObject& Node);

    private:
        /* ----------------------------- AST inspection ----------------------------- */
        inline static std::vector<std::string_view> NameSpaces;
        static bool InvalidIdentifier(const char& CharactherToCheck);
        
        static std::string MangleName(const Body& FunctionArguments, std::string& Name);
        static std::string MangleName(const std::vector<std::pair<std::string, NodeType>>& FunctionArguments, std::string& Name);

        /* ---------------------------- Repeated actions ---------------------------- */
        static void InspectTypesReferences(const NodeType& Type, const NodeObject& Node);
        static void AddVariableScope();
        static void DestroyVariableScope();
        inline static std::vector<VariableMap> Variables;
        inline static VariableMap* CurrentVariables;
        inline static std::vector<FunctionCall*> FunctionCalls;
        inline static const std::map<NodeType, std::string> ReverseTypeTable 
        {
            {NodeType::Reference, "ref"},
            {NodeType::Any,       "any"},
            {NodeType::Int64,     "int64"},
            {NodeType::Double,    "double"},
            {NodeType::String,    "string"},
            {NodeType::Bool,      "bool"},
        };
};

#endif // AST_INSPECTOR_HPP
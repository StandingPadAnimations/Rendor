#ifndef AST_INSPECTOR_HPP
#define AST_INSPECTOR_HPP

#include <filesystem>
#include <vector>
#include <memory>

#include "RendorCompiler/Nodes/NodeType.hpp"
#include "RendorCompiler/Nodes/BaseNode.hpp"
#include "RendorCompiler/Nodes/BodyNodes.hpp"
#include "RendorCompiler/Nodes/FunctionNodes.hpp"
#include "RendorCompiler/ASTInspection/RendorFunctionDeclaration.hpp"
#include "RendorAPI/RendorAPI.h"
#include "RendorAPI/FunctionTable.hpp"

#define BOOST_DLL_USE_STD_FS 1
#include <boost/dll.hpp>
#include <boost/predef.h>
#include <boost/dll/runtime_symbol_info.hpp>

#define FALLTHROUGH [[fallthrough]]
typedef std::map<std::string_view, NodeType> VariableMap;
typedef std::vector<NodeType> FunctionArgsVector;


class ASTInspector
{
    public:
        static void InspectAST(const NodeObject& Node);
        ASTInspector();

    private:
        /* ----------------------------- AST inspection ----------------------------- */
        inline static std::vector<std::string_view> NameSpaces;
        static bool InvalidIdentifier(const char& CharactherToCheck);
        static std::string MangleName(const Body& FunctionArguments, std::string& Name, NodeType& ReturnType);
        static std::string MangleName(const std::vector<std::pair<std::string, NodeType>>& FunctionArguments, std::string& Name, NodeType& ReturnType);

        /* ---------------------------- Repeated actions ---------------------------- */
        static void InspectTypesReferences(const NodeType& Type, const NodeObject& Node);
        static void AddVariableScope();
        static void DestroyVariableScope();
        inline static std::vector<VariableMap> Variables;
        inline static VariableMap* CurrentVariables;
        inline static std::set<std::string, std::less<>> CImports;

        /* -------------------------------- Functions ------------------------------- */
        inline static std::vector<Declaration> Declarations;
        inline static std::vector<FunctionCall*> FunctionCalls;

        /* --------------------------------- C++ API -------------------------------- */
        inline static FunctionTable CFunctionTable;
        inline static RendorState RendorEngineCompilerState;
        inline static std::string CurrentPath;
        #if BOOST_OS_WINDOWS
            inline static std::string C_Extension_Suffix = "dll";
        #else
            inline static std::string C_Extension_Suffix = "so";
        #endif
        inline static const std::map<NodeType, std::string, std::less<>> ReverseTypeTable 
        {
            {NodeType::Reference, "ref"},
            {NodeType::Any,       "any"},
            {NodeType::Int64,     "int64"},
            {NodeType::Double,    "double"},
            {NodeType::String,    "string"},
            {NodeType::Bool,      "bool"},
        };

        inline static const std::set<std::string_view> RendorSTDCPP 
        {
            {"Rendor_IO"},
            {"Rendor_Math"},
        };
};

#endif // AST_INSPECTOR_HPP
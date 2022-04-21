#ifndef AST_INSPECTOR_HPP
#define AST_INSPECTOR_HPP

#include <boost/dll/runtime_symbol_info.hpp>
#include <filesystem>
#include <vector>
#include <memory>

#include "RendorCompiler/Nodes/Nodes.hpp"
#include "RendorAPI/RendorAPI.hpp"

#define BOOST_DLL_USE_STD_FS 1
#include <boost/dll.hpp>
#include <boost/predef.h>

#define FALLTHROUGH [[fallthrough]]
typedef std::map<std::string_view, NodeType> VariableMap;
typedef std::vector<NodeType> FunctionArgsVector;


class ASTInspector
{
    public:
        static void InspectAST(const NodeObject& Node);
        static bool InitModule(RendorMethod* MethodList);
        ASTInspector();

    private:
        /* ----------------------------- AST inspection ----------------------------- */
        inline static std::vector<std::string_view> NameSpaces;
        static bool InvalidIdentifier(const char& CharactherToCheck);
        static std::string MangleName(const Body& FunctionArguments, std::string& Name);
        static std::string MangleName(const std::vector<std::pair<std::string, NodeType>>& FunctionArguments, std::string& Name);
        inline static std::unique_ptr<RendorState> RendorEngineCompilerState;

        /* ---------------------------- Repeated actions ---------------------------- */
        static void InspectTypesReferences(const NodeType& Type, const NodeObject& Node);
        static void AddVariableScope();
        static void DestroyVariableScope();
        inline static std::vector<VariableMap> Variables;
        inline static VariableMap* CurrentVariables;
        inline static std::vector<FunctionCall*> FunctionCalls;
        inline static std::set<std::string, std::less<>> CImports;

        /* --------------------------------- C++ CPI -------------------------------- */
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
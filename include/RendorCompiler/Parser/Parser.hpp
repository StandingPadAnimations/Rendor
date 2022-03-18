#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <map>

#include "RendorCompiler/Lexer/Lexer.hpp"
#include "RendorCompiler/Math/Mathparsing.hpp"
#include "RendorCompiler/Math/MathEvaluator.hpp"
#include "RendorCompiler/Parser/ASTGenerationFunctions.hpp"
#include "RendorCompiler/Nodes/Nodes.hpp"

#include "Exceptions.hpp"
#include "UnorderedMapLookUp.hpp"

#include <boost/format.hpp>

using lt = Lex::Token;
#define FALLTHROUGH [[fallthrough]]

typedef std::map<std::string_view, NodeType> VariableMap;
typedef std::vector<NodeType> FunctionArgsVector;
class Parser
{
    public:
        inline static Main Script;
        static void ASTGeneration(const std::vector<std::pair<Lex::Token, std::string>>& Tokens);
    
    private:

        /* ----------------------------- AST generation ----------------------------- */
        inline static std::vector<std::vector<std::unique_ptr<Node>>*> ScopeList {&Script.Global.ConnectedNodes}; // Scopes
        inline static std::vector<TempID> ParserTempIDList {TempID::None};
        inline static TempID ParserTempID = ParserTempIDList.back();
        inline static std::vector<std::unique_ptr<Node>>* Scope = ScopeList.back();
        inline static uint32_t LineNumber = 0;
        inline static bool IsScript = false;

        static void PopTempID();
        static void AddTempID(TempID ID);
        static void PopScope();
        static void AddScope(std::vector<std::unique_ptr<Node>>* Ptr);
        static void PushToScope(std::unique_ptr<Node> Node);
        static void ReplaceNode(std::unique_ptr<Node> Node);
        static NodeType GetTypeOfNode();

        /* --------------------------- Bytecode generation -------------------------- */
        static std::string ByteCodeGen(const NodeType& ClassType, const NodeObject& NodeClass);
        static void TypeConstants(const NodeType& ClassType, const NodeObject& Node);

        /* ----------------------------- AST inspection ----------------------------- */
        static void DeltaInspectAST(const NodeObject& Node);
        static bool InvalidIdentifier(char& CharactherToCheck);

        /* ---------------------------- Repeated actions ---------------------------- */
        static void InspectTypesReferences(const NodeType& Type, const NodeObject& Node);
        static void AddVariableScope();
        static void DestroyVariableScope();

        inline static std::vector<VariableMap> Variables;
        inline static VariableMap* CurrentVariables;
        inline static std::vector<FunctionCall*> FunctionCalls;
        inline static std::unordered_map<std::string_view, FunctionArgsVector, string_hash, std::equal_to<>> Functions {{"echo",  {NodeType::String}},
                                                                                                                        {"input", {NodeType::String}},
                                                                                                                        {"sum",   {NodeType::Int}}};
};


#endif // PARSER_HPP
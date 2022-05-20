#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>
#include <map>
#include <set>

#include "RendorCompiler/Lexer/Lexer.hpp"
#include "RendorCompiler/Math/Mathparsing.hpp"
#include "RendorCompiler/Math/MathEvaluator.hpp"
#include "RendorCompiler/Parser/ASTGenerationFunctions.hpp"
#include "RendorCompiler/Parser/TempID.hpp"
#include "RendorCompiler/Nodes/Nodes.hpp"
#include "RendorCompiler/ASTInspection/ASTInspector.hpp"

#include "RendorEngine.hpp"
#include "Exceptions.hpp"
#include "UnorderedMapLookUp.hpp"

using lt = Lex::Token;
#define FALLTHROUGH [[fallthrough]]

typedef std::map<std::string_view, NodeType> VariableMap;
typedef std::vector<NodeType> FunctionArgsVector;
class Parser
{
    public:
        inline static Main Script;
        static void ASTGeneration(const std::vector<std::pair<Lex::Token, std::string>>& Tokens);

        Parser()
        {
            ParserTempIDList.reserve(5);
        }
    private:

        /* ----------------------------- AST generation ----------------------------- */
        inline static std::vector<Body*> ScopeList {&Script.Global}; // Scopes
        inline static std::vector<Body*> ParentNodes;
        inline static std::vector<TempID> ParserTempIDList {TempID::None};
        inline static TempID ParserTempID = ParserTempIDList.back();
        inline static Body* Scope = ScopeList.back();
        inline static uint32_t LineNumber = 1;
        inline static bool IsScript = false;

        inline static const std::map<std::string_view, NodeType> TypeTable 
        {
            {"int64",  NodeType::Int64},
            {"double", NodeType::Double},
            {"string", NodeType::String},
            {"bool",   NodeType::Bool},
        };

        static void PopTempID();
        static void AddTempID(TempID ID);
        static void PopScope();
        static void AddScope(Body* Ptr);
        static void PushToScope(std::unique_ptr<Node> Node);
        static void ReplaceNode(std::unique_ptr<Node> Node);
        static NodeType GetTypeOfNode();
        static void AddParentNode(Body* ParentNode);
};


#endif // PARSER_HPP
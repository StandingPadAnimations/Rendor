#ifndef PARSER
#define PARSER
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <map>

#include "RendorCompiler/Lexer.hpp"
#include "RendorCompiler/Mathparsing.hpp"
#include "RendorCompiler/MathEvaluator.hpp"
#include "RendorCompiler/RendorDeltaOptimizer.hpp"
#include "RendorCompiler/ASTGenerationFunctions.hpp"
#include "RendorCompiler/Nodes.hpp"
#include "Exceptions.hpp"

#include <boost/format.hpp>

using lt = Lex::Token;
#define FALLTHROUGH [[fallthrough]]

class Parser
{
    public:
        static std::vector<std::string> ASTGeneration(const std::vector<std::pair<Lex::Token, std::string>>& Tokens);
    
    private:
        inline static Main Script;
        inline static std::vector<std::string> ByteCode;

        /* --------------------------- Bytecode generation -------------------------- */
        static std::string ByteCodeGen(const NodeType& ClassType, const NodeObject& NodeClass);
        static void TypeConstants(const NodeType& ClassType, const NodeObject& Node);

        /* ----------------------------- AST inspection ----------------------------- */
        static void DeltaInspectAST(const NodeObject& Node);
        static bool InvalidIdentifier(char& CharactherToCheck);
        static void InspectTypesReferences(const NodeType& Type, const NodeObject& Node);

        inline static std::map<std::string_view, NodeType> Variables;
        inline static std::map<std::string_view, std::vector<NodeType>> Functions;
};


#endif // * PARSER
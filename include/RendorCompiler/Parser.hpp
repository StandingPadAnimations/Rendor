#ifndef PARSER
#define PARSER
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "RendorCompiler/Lexer.hpp"
#include "RendorCompiler/Mathparsing.hpp"
#include "RendorCompiler/MathEvaluator.hpp"
#include "RendorCompiler/RendorDeltaOptimizer.hpp"
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
        static std::string ByteCodeGen(const NodeType& ClassType, const NodeObject& NodeClass);
        static void TypeConstants(const NodeType& ClassType, const NodeObject& Node);
        static void DeltaInspectAST(const NodeObject& Node);
};


#endif // * PARSER
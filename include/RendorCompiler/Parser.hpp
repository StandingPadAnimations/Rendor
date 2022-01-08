#ifndef PARSER
#define PARSER
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "RendorCompiler/Lexer.hpp"
#include "RendorCompiler/Mathparsing.hpp"
#include "Exceptions.hpp"

#include <boost/format.hpp>

using lt = Lex::Token;

std::vector<std::string> Parser(const std::vector<std::pair<Lex::Token, std::string>>& Tokens);


enum class VariableTypes{
    Int, 
    Float,
    String,
    Bool,
    Arith,
    Function,
    IntStatic,
    FloatStatic,
    StringStatic,
    BoolStatic
};

enum class TempID{
    None,

    // Definitions
    ArithAssemble,
    IdentifierDefinition,
    VariableDefition,
    FunctionDefiniton,
    FunctionArgumentsDefinition,

    // Function things
    FunctionCall,
    FunctionScope,

    // If-else statements 
    IfStatementDefinition,
    ConditionDefinition,
    IfElseScope
};

enum class NodeType{
    Body,
    Scope,
    Rdef,
    MarkRdef,
    MarkGlobal,
    AssignVariable,
    IfElse,
    Increment,
    Decrement,
    FunctionCall,
    Edef,
};

struct Node{
    // virtual function; is used for some things
    virtual NodeType Type() = 0;
};

struct Body : Node{
    std::vector<std::unique_ptr<Node>> ConnectedNodes;
    NodeType Type(){return NodeType::Body;}
};

struct Scope : Node{
    std::string ScopeName;
    Body ScopeBody;
    NodeType Type(){return NodeType::Scope;}
};

struct AssignVariable : Node{
    std::string VariableName;
    std::string Value;
    VariableTypes VariableType;

    explicit AssignVariable(std::string VariableName) : VariableName(VariableName){}
    NodeType Type(){return NodeType::AssignVariable;}
};

struct IfElse : Node{
    std::vector<std::string> Conditions;
    Body IfElseBody;
    std::vector<std::unique_ptr<Node>> *ScopedBody = &IfElseBody.ConnectedNodes;
    NodeType Type(){return NodeType::IfElse;}
};

struct FunctionCall : Node{
    std::string Function;
    std::vector<std::string> Args;
    explicit FunctionCall(std::string Function) : Function(Function){}
    NodeType Type(){return NodeType::FunctionCall;}
};

struct Increment : Node{
    std::string Args;
    explicit Increment(std::string Args) : Args(Args){}
    NodeType Type(){return NodeType::Increment;}
};

struct Decrement : Node{
    std::string Args;
    explicit Decrement(std::string Args) : Args(Args){}
    NodeType Type(){return NodeType::Decrement;}
};

struct Main{
    Body Global;
    std::vector<std::unique_ptr<Node>> *GlobalBody = &Global.ConnectedNodes;
};

struct Edef : Node{
    std::string Name;
    Body FunctionBody;
    std::vector<std::string> Args;
    std::vector<std::unique_ptr<Node>> *ScopedBody = &FunctionBody.ConnectedNodes;

    explicit Edef(std::string Name) : Name(Name){}
    NodeType Type(){return NodeType::Edef;}
};
#endif // * PARSER
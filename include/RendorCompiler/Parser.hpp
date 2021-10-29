#ifndef PARSER
#define PARSER
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <boost/format.hpp>
#include "RendorCompiler/Lexer.hpp"
#include "Exceptions.hpp"

std::vector<std::string> Parser(const std::vector<std::pair<Lex::Token, std::string>>& Tokens);


enum class VariableTypes{
    Int, 
    Float,
    String,
    Bool,
    IntStatic,
    FloatStatic,
    StringStatic,
    BoolStatic
};

enum class CurrentScope{
    Global,
    Main,
};

enum class NodeType{
    Body,
    Scope,
    Rdef,
    MarkRdef,
    MarkGlobal,
    AssignVariable,
    RendorKeyWord
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

struct Rdef : Node{
    Body MainFunctionBody;
    NodeType Type(){return NodeType::Rdef;}
};

struct MarkRdef : Node{
    NodeType Type(){return NodeType::MarkRdef;}
};

struct MarkGlobal : Node{
    NodeType Type(){return NodeType::MarkGlobal;}
};

struct AssignVariable : Node{
    std::string VariableName;
    std::string Value;
    VariableTypes VariableType;

    explicit AssignVariable(std::string VariableName) : VariableName(VariableName){}
    NodeType Type(){return NodeType::AssignVariable;}
};

struct RendorKeyWord : Node{
    std::string KeyWord;
    std::string Args;
    explicit RendorKeyWord(std::string KeyWord) : KeyWord(KeyWord){}
    NodeType Type(){return NodeType::RendorKeyWord;}
};

struct Main{
    Body Global;
    Rdef MainFunction;
    std::vector<std::unique_ptr<Node>> *GlobalBody = &Global.ConnectedNodes;
    std::vector<std::unique_ptr<Node>> *MainBody = &MainFunction.MainFunctionBody.ConnectedNodes;
};
#endif // * PARSER
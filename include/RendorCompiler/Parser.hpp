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

std::vector<std::string> Parser(const std::vector<std::pair<Lex::Token, std::string>>& Tokens);


enum class VariableTypes{
    Int, 
    Float,
    String,
    Bool,
    Arith,
    IntStatic,
    FloatStatic,
    StringStatic,
    BoolStatic
};

enum class TempID{
    None,
    ArithAssemble
};

enum class NodeType{
    Body,
    Scope,
    Rdef,
    MarkRdef,
    MarkGlobal,
    AssignVariable,
    RendorKeyWord,
    Increment,
    Decrement
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
    Rdef MainFunction;
    std::vector<std::unique_ptr<Node>> *GlobalBody = &Global.ConnectedNodes;
    std::vector<std::unique_ptr<Node>> *MainBody = &MainFunction.MainFunctionBody.ConnectedNodes;
};
#endif // * PARSER
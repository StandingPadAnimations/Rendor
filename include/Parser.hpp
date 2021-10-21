#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "boost/format.hpp"
#include "Lexer.hpp"
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

struct Node{
    // virtual function; is used for some things
    virtual std::string Type() = 0;
};

struct Body : Node{
    std::vector<std::unique_ptr<Node>> ConnectedNodes;
    std::string Type(){return "body";}
};

struct Scope : Node{
    std::string ScopeName;
    Body ScopeBody;
    std::string Type(){return "scope";}
};

struct Rdef : Node{
    Body MainFunctionBody;
    std::string Type(){return "rdef";}
};

struct MarkRdef : Node{
    std::string Type(){return "mark_rdef";}
};

struct MarkGlobal : Node{
    std::string Type(){return "mark_global";}
};

struct AssignVariable : Node{
    std::string VariableName;
    std::string Value;
    VariableTypes VariableType;

    explicit AssignVariable(std::string VariableName) : VariableName(VariableName){}
    std::string Type(){return "assignment";}
};

struct Main{
    Body Global;
    Rdef MainFunction;
    std::vector<std::unique_ptr<Node>> *GlobalBody = &Global.ConnectedNodes;
    std::vector<std::unique_ptr<Node>> *MainBody = &MainFunction.MainFunctionBody.ConnectedNodes;
};

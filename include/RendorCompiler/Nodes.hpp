#ifndef RENDOR_NODES_HPP
#define RENDOR_NODES_HPP

#include <string>
#include <vector>
#include <memory>

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
    ElseDefinition,
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
    Condition,
    BiOp,

    Any,
    Int, 
    Double,
    String,
    Bool,
    Reference,
    Arithmethic,
};

struct Node{
    NodeType Type;
    uint32_t LineNumber = 1;
    
    Node() = default;
    Node(uint32_t LineNumber) : LineNumber(LineNumber){}
    virtual ~Node(){};
};

typedef std::unique_ptr<Node> NodeObject;

struct Body : Node
{
    std::vector<NodeObject> ConnectedNodes;
    Body()
    {
        Type = NodeType::Body;
    }
};

struct Scope : Node
{
    std::string ScopeName;
    Body ScopeBody;
    Scope(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Scope;
    }
};

struct AssignVariable : Node
{
    std::string VariableName;
    NodeObject Value;

    AssignVariable() = default;
    explicit AssignVariable(std::string VariableNameInput, uint32_t LineNumber) : Node(LineNumber), VariableName(VariableNameInput)
    {
        Type = NodeType::AssignVariable;
    }
};

struct FunctionCall : Node
{
    std::string Function;
    std::vector<NodeObject> Args;
    explicit FunctionCall(std::string Function, uint32_t LineNumber) : Node(LineNumber), Function(Function)
    {
        Type = NodeType::FunctionCall;
    }
};

struct Increment : Node
{
    std::string Args;
    explicit Increment(std::string Args, uint32_t LineNumber) : Node(LineNumber), Args(Args)
    {
        Type = NodeType::Increment;
    }
};

struct Decrement : Node
{
    std::string Args;
    explicit Decrement(std::string Args, uint32_t LineNumber) : Node(LineNumber), Args(Args)
    {
        Type = NodeType::Decrement;
    }
};

struct Edef : Node
{
    std::string Name;
    Body FunctionBody;
    std::vector<std::pair<std::string, NodeType>> Args;

    explicit Edef(std::string Name, uint32_t LineNumber) : Node(LineNumber), Name(Name)
    {
        Type = NodeType::Edef;
    }
};

struct BiOp : Node
{
    std::string Operator;

    BiOp(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::BiOp;
    }
};

struct Condition : Node
{
    NodeObject Condition1;
    std::unique_ptr<BiOp> Operator;
    NodeObject Condition2;

    Condition(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Condition;
    }
};

struct IfElse : Node
{
    std::vector<std::unique_ptr<Condition>> Conditions;
    Body IfElseBody;
    std::unique_ptr<IfElse> ElseStatement;
    IfElse(uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::IfElse;
    }
};

struct Int : Node
{
    std::string Value;
    
    Int(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Int;
    }
};

struct Double : Node
{
    std::string Value;
    
    Double(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Double;
    }
};

struct String : Node
{
    std::string Value;
    
    String(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::String;
    }
};

struct Bool : Node
{
    std::string Value;
    
    Bool(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Bool;
    }
};

struct Arithmethic : Node
{
    std::string Value;
    
    Arithmethic(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Arithmethic;
    }
};

struct Reference : Node
{
    std::string Value;

    Reference(std::string ValueInput, uint32_t LineNumber) : Node(LineNumber)
    {
        Type = NodeType::Reference;
    }
};


struct Main
{
    Body Global;
    std::vector<NodeObject> *GlobalBody = &Global.ConnectedNodes;
};
#endif // RENDOR_NODES_HPP
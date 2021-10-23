#ifndef INTERPRETER
#define INTERPRETER
#include <string>
#include <memory>

void ExecuteByteCode();

enum class VariableType{
    Int,
    Float,
    String,
    Bool
};

struct Variable{
    std::string Name;
    std::unique_ptr<Type> ValueClass;
};

struct Type {
    virtual VariableType TypeOfVariable() = 0;
    protected:
        std::string Value;
        explicit Type(const std::string& Value) : Value(Value){}
};
struct Int : Type{
    VariableType TypeOfVariable(){return VariableType::Int;}
    explicit Int(std::string Value) : Type(Value){}
};
struct Float : Type{
    VariableType TypeOfVariable(){return VariableType::Int;}
    explicit Float(std::string Value) : Type(Value){}
};

#endif // INTERPRETER

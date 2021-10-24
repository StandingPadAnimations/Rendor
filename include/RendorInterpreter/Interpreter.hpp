#ifndef INTERPRETER
#define INTERPRETER
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <map>
#include "Exceptions.hpp"
#include "ExtraFunctions.hpp"

void ExecuteByteCode(std::ifstream& File);


enum class VariableType{
    Int,
    Float,
    String,
    Bool
};

struct Type {
    virtual VariableType TypeOfVariable() = 0;
    public:
        std::string Value;
        explicit Type(const std::string& Value) : Value(Value){}
};

struct Int : Type{
    VariableType TypeOfVariable(){return VariableType::Int;}
    explicit Int(std::string Value) : Type(Value){}
    
    inline int RetriveVariable(){
        return std::stoi(Value);
    }
};

struct Float : Type{
    VariableType TypeOfVariable(){return VariableType::Float;}
    explicit Float(std::string Value) : Type(Value){}

    inline float RetriveVariable(){
        return std::stof(Value);
    }
};

struct String : Type{
    VariableType TypeOfVariable(){return VariableType::String;}
    explicit String(std::string Value) : Type(Value){}

    inline std::string RetriveVariable(){
        return Value;
    }
};

struct Bool : Type{
    VariableType TypeOfVariable(){return VariableType::Bool;}
    explicit Bool(std::string Value) : Type(Value){}

    inline bool RetriveVariable(){
        if(Value == "true"){
            return true;
        }
        else{
            return false;
        }
    }
};

struct Variable{
    std::string Name;
    std::unique_ptr<Type> ValueClass;
    explicit Variable(std::string Name) : Name(Name){}
};


#endif // INTERPRETER

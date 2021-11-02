#ifndef INTERPRETER
#define INTERPRETER
#include <string>
#include <memory>
#include <fstream>
#include <map>
#include <algorithm>

#include "Exceptions.hpp"
#include "ExtraFunctions.hpp"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/finder.hpp>

void ExecuteByteCode(std::ifstream& File);


enum class VariableType{
    Int,
    Float,
    String,
    Bool,
    NullType,
    Constant
};

struct Type {
    virtual VariableType TypeOfVariable() = 0;
    public:
        std::string Value;
        explicit Type(const std::string& Value) : Value(Value){}
};

struct NullType : Type{
    VariableType TypeOfVariable(){return VariableType::NullType;}
    
    inline std::string RetriveVariable(){
        return "Null";
    }
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

struct Constant : Type{
    VariableType TypeOfVariable(){return VariableType::Constant;}
    char ConstVariableType;
    explicit Constant(std::string Value, char ConstVariableType) : Type(Value), ConstVariableType(ConstVariableType){}

    inline std::string RetriveVariable(){
        return Value;
    }
};

struct Variable{
    std::string Name;
    std::unique_ptr<Type> ValueClass;
    explicit Variable(std::string Name) : Name(Name){}
};


#endif // INTERPRETER

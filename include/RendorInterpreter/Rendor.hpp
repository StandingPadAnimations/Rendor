#ifndef RENDORAPI
#define RENDORAPI 

#include <iostream>
#include <string>

#include "Interpreter.hpp"

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
    
    std::string m_Value;
    explicit Type(const std::string& Value) : m_Value(Value){}
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
        return std::stoi(m_Value);
    }
};

struct Float : Type{
    VariableType TypeOfVariable(){return VariableType::Float;}
    explicit Float(std::string Value) : Type(Value){}

    inline float RetriveVariable(){
        return std::stof(m_Value);
    }
};

struct String : Type{
    VariableType TypeOfVariable(){return VariableType::String;}
    explicit String(std::string Value) : Type(Value){}

    inline std::string RetriveVariable(){
        return m_Value;
    }
};

struct Bool : Type{
    VariableType TypeOfVariable(){return VariableType::Bool;}
    explicit Bool(std::string Value) : Type(Value){}

    inline bool RetriveVariable(){
        if(m_Value == "true"){
            return true;
        }
        else{
            return false;
        }
    }
};

struct Variable{
    std::string m_Name;
    std::unique_ptr<Type> m_ValueClass;
    explicit Variable(std::string Name) : m_Name(Name){}
};

#endif // Rendor API
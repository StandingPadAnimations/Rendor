#ifndef RENDORAPI_H
#define RENDORAPI_H 

#include <iostream>
#include <string>
#include <variant>
#include <memory>

#include "Exceptions.hpp"

#include <boost/lexical_cast.hpp>

enum class VariableType{
    Int,
    Float,
    String,
    Bool,
    NullType,
    Constant,
    Function
};

enum class Operator{
    EQUAL,
    NOT_EQUAL,
    GREATER_THAN,
    LESS_THAN,
    GREATER_OR_EQUAL,
    LESS_OR_EQUAL
};


struct Type 
{
    virtual VariableType TypeOfVariable(){return VariableType::NullType;};
    virtual bool IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator){return false;};

    std::string m_Value;

    explicit Type(const std::string& Value) : m_Value(Value){}
    Type() = default;
    virtual ~Type(){};
};  

struct NullType : Type
{
    VariableType TypeOfVariable(){return VariableType::NullType;}
    
    std::string RetriveVariable()
    {
        return "Null";
    }
};

/* -------------------------------------------------------------------------- */
/*                                     Int                                    */
/* -------------------------------------------------------------------------- */
struct Int : Type
{
    VariableType TypeOfVariable(){return VariableType::Int;}
    explicit Int(std::string Value) : Type(Value){}
    
    int64_t RetriveVariable()
    {
        return boost::lexical_cast<int64_t>(m_Value);
    }
    bool IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator); 
};

/* -------------------------------------------------------------------------- */
/*                                    Float                                   */
/* -------------------------------------------------------------------------- */
struct Float : Type
{
    VariableType TypeOfVariable(){return VariableType::Float;}
    explicit Float(std::string Value) : Type(Value){}

    float RetriveVariable()
    {
        return std::stod(m_Value);
    }
    bool IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator);
};

struct String : Type
{
    VariableType TypeOfVariable(){return VariableType::String;}
    explicit String(std::string Value) : Type(Value){}

    std::string RetriveVariable()
    {
        return m_Value;
    }
    bool IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator);
};

struct Bool : Type
{
    VariableType TypeOfVariable(){return VariableType::Bool;}
    explicit Bool(std::string Value) : Type(Value){}

    bool RetriveVariable()
    {
        if(m_Value == "true"){
            return true;
        }
        else{
            return false;
        }
    }
    bool IfStatementMethod(std::unique_ptr<Type>& Const2, Operator ComparisonOperator);
};

struct Variable
{
    std::string m_Name;
    std::shared_ptr<Type> m_ValueClass;
    explicit Variable(std::string Name) : m_Name(Name){}
};

#endif // RENDORAPI_H
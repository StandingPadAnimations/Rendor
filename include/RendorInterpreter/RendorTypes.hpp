#ifndef RENDOR_TYPES_HPP
#define RENDOR_TYPES_HPP 

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

enum class GCColor
{
    WHITE,
    GREY,
    BLACK
};

struct Type 
{
    virtual VariableType TypeOfVariable(){return VariableType::NullType;};
    virtual bool IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator) = 0;

    std::string m_Value;
    std::string m_ID;
    std::vector<std::string_view> IncomingPointers;
    GCColor ColorOfObj = GCColor::WHITE;

    explicit Type(const std::string& Value);
    virtual ~Type(){};

    private:
        inline static uint64_t IntForID = 0;
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
    int64_t ConvertedValue;
    explicit Int(std::string Value);
    VariableType TypeOfVariable(){return VariableType::Int;}
    bool IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator); 
};

/* -------------------------------------------------------------------------- */
/*                                    Float                                   */
/* -------------------------------------------------------------------------- */
struct Float : Type
{
    double ConvertedValue;
    explicit Float(std::string Value);

    VariableType TypeOfVariable(){return VariableType::Float;}
    bool IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator);
};

struct String : Type
{
    explicit String(std::string Value) : Type(Value){}

    VariableType TypeOfVariable(){return VariableType::String;}
    bool IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator);
};

struct Bool : Type
{
    bool ConvertedValue;
    explicit Bool(std::string Value);
    VariableType TypeOfVariable(){return VariableType::Bool;}
    bool IfStatementMethod(std::shared_ptr<Type>& Const2, Operator ComparisonOperator);
};

#endif // RENDOR_TYPES_HPP
#ifndef RENDOR_TYPES_HPP
#define RENDOR_TYPES_HPP 

#include <iostream>
#include <string>
#include <variant>
#include <memory>

#include "Exceptions.hpp"

#include <boost/lexical_cast.hpp>

enum class VariableType
{
    Int64,
    Double,
    String,
    Bool,
    NullType,
    Constant,
    Function
};

enum class Operator
{
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
    virtual VariableType TypeOfVariable() {return VariableType::NullType;};
    virtual bool IfStatementMethod(Type* Const2, Operator ComparisonOperator) = 0;

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
    VariableType TypeOfVariable() override {return VariableType::NullType;}
    
    std::string RetriveVariable()
    {
        return "Null";
    }
};

/* -------------------------------------------------------------------------- */
/*                                     Int                                    */
/* -------------------------------------------------------------------------- */
struct Int64 : Type
{
    int64_t ConvertedValue;
    explicit Int64(std::string Value);
    VariableType TypeOfVariable() override {return VariableType::Int64;}
    bool IfStatementMethod(Type* Const2, Operator ComparisonOperator) override; 
};

/* -------------------------------------------------------------------------- */
/*                                    Float                                   */
/* -------------------------------------------------------------------------- */
struct Double : Type
{
    double ConvertedValue;
    explicit Double(std::string Value);

    VariableType TypeOfVariable() override {return VariableType::Double;}
    bool IfStatementMethod(Type* Const2, Operator ComparisonOperator) override;
};

struct String : Type
{
    explicit String(std::string& Value) : Type(Value){}

    VariableType TypeOfVariable() override {return VariableType::String;}
    bool IfStatementMethod(Type* Const2, Operator ComparisonOperator) override;
};

struct Bool : Type
{
    bool ConvertedValue;
    explicit Bool(std::string Value);
    VariableType TypeOfVariable() override {return VariableType::Bool;}
    bool IfStatementMethod(Type* Const2, Operator ComparisonOperator)override;
};

#endif // RENDOR_TYPES_HPP
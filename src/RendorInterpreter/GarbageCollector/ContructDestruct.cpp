#include "RendorInterpreter/Interpreter.hpp"
#include "RendorInterpreter/VariableType.hpp"
#include "RendorInterpreter/RendorTypes.hpp"

Variable::Variable(std::string Name) : m_Name(Name)
{
    m_ID = std::to_string(IntForID) + "_RENDOR_VAR";
    ++IntForID;
}


Variable::~Variable()
{
    if (m_ValueClass.use_count() == 2)
    {
        Interpreter::DisposeConst(m_ValueClass);
    }
}

Type::Type(const std::string& Value) : m_Value(Value)
{
    m_ID = std::to_string(IntForID) + "_RENDOR_CONST";
    ++IntForID;
}

Int64::Int64(std::string Value) : Type(Value)
{
    ConvertedValue = boost::lexical_cast<int64_t>(Value);
}

Double::Double(std::string Value) : Type(Value)
{
    ConvertedValue = std::stod(Value);
}

Bool::Bool(std::string Value) : Type(Value)
{
    if (m_Value == "true")
    {
        ConvertedValue = true;
    } else
    {
        ConvertedValue = false;
    }
}
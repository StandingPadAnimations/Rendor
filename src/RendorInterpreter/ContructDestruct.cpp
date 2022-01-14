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
    m_ValueClass->ColorOfObj = GCColor::WHITE;
    m_ValueClass->IncomingPointers.erase(std::remove(m_ValueClass->IncomingPointers.begin(), m_ValueClass->IncomingPointers.end(), m_ID));
    Interpreter::MarkConstIDForDisposal(m_ValueClass->m_ID);
}

Type::Type(const std::string& Value) : m_Value(Value)
{
    m_ID = std::to_string(IntForID) + "_RENDOR_CONST";
    ++IntForID;
}

Int::Int(std::string Value) : Type(Value)
{
    ConvertedValue = boost::lexical_cast<int64_t>(Value);
}

Float::Float(std::string Value) : Type(Value)
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
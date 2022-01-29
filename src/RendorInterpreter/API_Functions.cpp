#include "RendorInterpreter/Interpreter.hpp"

void Interpreter::DisposeConst(TypeObject Const)
{
    switch (Const->ColorOfObj)
    {
        case GCColor::BLACK:
        {
            auto OriginalPtr = std::find(BlackObjects.begin(), BlackObjects.end(), Const);
            WhiteObjects.push_back(std::move(*OriginalPtr));
            BlackObjects.erase(OriginalPtr);
            break;
        }

        case GCColor::GREY:
        {
            auto OriginalPtr = std::find(GreyObjects.begin(), GreyObjects.end(), Const);
            WhiteObjects.push_back(std::move(*OriginalPtr));
            GreyObjects.erase(OriginalPtr);
            break;
        }

        default:
        {
            break;
        }
    }
    Const->ColorOfObj = GCColor::WHITE;
}

TypeObjectPtr Interpreter::GetConstFromVariable(const std::string& Variable)
{
    /* ------------------------ Check if variable exists ------------------------ */
    if (GlobalVariables->contains(Variable))
    {
        return (*GlobalVariables)[Variable]->m_ValueClass;
    }
    else if (CurrentScopeVariables->contains(Variable))
    {
        return (*CurrentScopeVariables)[Variable]->m_ValueClass;
    }
    else 
    {
        throw error::RendorException("Variable " + Variable + " doesn't exist!");
    }

    return TypeObjectPtr();
}
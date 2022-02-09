#include "RendorInterpreter/Interpreter.hpp"

/* -------------------------------------------------------------------------- */
/*                          Memory related functions                          */
/* -------------------------------------------------------------------------- */
/* ---------------------------- Garbage Collector --------------------------- */
void Interpreter::GarbageCollector()
{
    WhiteObjects.clear();
    for (auto Constant : GreyObjects)
    {
        BlackObjects.push_back(std::move(Constant));
        break;
    }

    /* ------------------------ Remove all empty pointers ----------------------- */
    if (Objects.size())
    {
        Objects.erase(std::remove_if(Objects.begin(), Objects.end(), 
        [](TypeObjectPtr Ptr)
        {
            return Ptr.expired() || Ptr.lock()->ColorOfObj == GCColor::WHITE;
        }));
    }
    WhiteObjects.clear();
}


void Interpreter::MarkConstantBlack(TypeObject Const)
{
    if (Const.use_count()){
        switch (Const->ColorOfObj)
        {
            case GCColor::BLACK:
            {
                break;
            }

            case GCColor::GREY:
            {
                auto OriginalPtr = std::find(GreyObjects.begin(), GreyObjects.end(), Const);
                BlackObjects.push_back(std::move(*OriginalPtr));
                break;
            }

            default:
            {
                auto OriginalPtr = std::find(WhiteObjects.begin(), WhiteObjects.end(), Const);
                BlackObjects.push_back(std::move(*OriginalPtr));
                break;
            }
        }
        Const->ColorOfObj = GCColor::BLACK;
    }
}

void Interpreter::AddToConstantsArray(TypeTuple ConstantToBePlaced)
{
    auto [Const1, Const2] = ConstantToBePlaced;
    AddToConstantsArray(Const1);
    AddToConstantsArray(Const2);
}

void Interpreter::AddToConstantsArray(TypeObjectPtr ConstantToBePlaced)
{
    if (!ConstantToBePlaced.expired())
    {
        if (Constants.size() == 2)
        {
            switch (ConstantIndex)
            {
                case 0:
                {
                    ++ConstantIndex;
                    break;
                }
                
                case 1:
                {
                    --ConstantIndex;
                    break;
                }
            }
            Constants[ConstantIndex] = ConstantToBePlaced;
        }
        else if (Constants.size() == 1)
        {
            Constants[1] = (ConstantToBePlaced);
            ++ConstantIndex;
        }
        else 
        {
            Constants[0] = (ConstantToBePlaced);
        }
    }
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

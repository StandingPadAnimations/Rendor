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

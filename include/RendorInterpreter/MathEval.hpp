#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP
#include <string>
using std::string;
#include "RendorInterpreter/Resources/RendorDefinitions.hpp"

// Code from Rythm#6156 with slight changes 
inline int64_t Rythm_RendorPow(int64_t& x, int64_t& y){
    int64_t ret = 1;
    for(; y; y >>= 1){
        if(y & 1)
        {
            ret *= x;
        } 
        x *= x;
    }
    return ret;
}

inline std::string EvalNums(MathVariant Num1, MathVariant Num2, std::string Op)
{
    std::string Answer;
    if (Op == "^")
    {
        if (
        (Num1.index() == 0) &&
        (Num2.index() == 0))
        {
            Answer = std::visit([&](int64_t Num1v, int64_t Num2v) { 
                return std::to_string(Rythm_RendorPow(Num1v, Num2v)); 
            }, Num1, Num2);
        }
        else 
        {
            Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return std::to_string(pow(Num1v, Num2v)); 
            }, Num1, Num2);
        }
    }

    else if (Op == "+")
    {
        Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return std::to_string(Num1v + Num2v); 
            }, Num1, Num2);
    }
    
    else if (Op == "-")
    {
        Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return std::to_string(Num1v - Num2v); 
            }, Num1, Num2);
    }

    else if (Op == "*")
    {
        Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return std::to_string(Num1v * Num2v); 
            }, Num1, Num2);
    }
            
    else if (Op == "/")
    {
        Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return std::to_string(Num1v / Num2v); 
            }, Num1, Num2);
    }
    return Answer;
}

template <typename T1>
inline void SetVariant(T1& Variant, TypeObjectPtr& TypeObject)
{
    switch (TypeObject.lock()->TypeOfVariable())
    {
        case VariableType::Int:
        {
            auto& IntNode = static_cast<Int&>(*TypeObject.lock());
            Variant = IntNode.ConvertedValue;
            break;
        }
        case VariableType::Float:
        {
            auto& FloatNode = static_cast<Float&>(*TypeObject.lock());
            Variant = FloatNode.ConvertedValue;
            break;
        }
        default:
        {
            break;
        }
    }
}

#endif // MATH_EVAL_HPP
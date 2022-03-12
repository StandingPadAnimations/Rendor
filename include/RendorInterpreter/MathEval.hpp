#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP
#include <string>
#include "RendorInterpreter/RendorDefinitions.hpp"

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

inline MathVariant EvalNums(MathVariant Num1, MathVariant Num2, std::string Op)
{
    MathVariant Answer;
    if (Op == "^")
    {
        if (
        (Num1.index() == 0) &&
        (Num2.index() == 0))
        {
            Answer = std::visit([&](int64_t Num1v, int64_t Num2v) { 
                return MathVariant(Rythm_RendorPow(Num1v, Num2v)); 
            }, Num1, Num2);
        }
        else 
        {
            Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return MathVariant(pow(Num1v, Num2v)); 
            }, Num1, Num2);
        }
    }

    else if (Op == "+")
    {
        Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return MathVariant(Num1v + Num2v); 
            }, Num1, Num2);
    }
    
    else if (Op == "-")
    {
        Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return MathVariant(Num1v - Num2v); 
            }, Num1, Num2);
    }

    else if (Op == "*")
    {
        Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return MathVariant(Num1v * Num2v); 
            }, Num1, Num2);
    }
            
    else if (Op == "/")
    {
        Answer = std::visit([&](auto Num1v, auto Num2v) { 
                return MathVariant(Num1v / Num2v); 
            }, Num1, Num2);
    }
    return Answer;
}

#endif // MATH_EVAL_HPP
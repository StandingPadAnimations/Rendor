#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <cstdint>
#include <string>
#include <map>

#include "Definitions.hpp"


struct Constant
{
    RendorVariant Const = false;
    RendorMathVariant Num = 0;

    void operator=(RendorVariant value)
    {
        Const = std::move(value);
    }

    /* ----------------------------------- add ---------------------------------- */
    void operator+(RendorMathVariant Val)
    {
        Num = std::visit([&](auto RegVal, auto ActualVal)
                {
                    return RendorMathVariant{RegVal + ActualVal};
                }, Num, Val);
    }

    /* ----------------------------------- sub ---------------------------------- */
    void operator-(RendorMathVariant Val)
    {
        Num = std::visit([&](auto RegVal, auto ActualVal)
                {
                    return RendorMathVariant{RegVal - ActualVal};
                }, Num, Val);
    }

    /* ----------------------------------- mul ---------------------------------- */
    void operator*(RendorMathVariant Val)
    {
        Num = std::visit([&](auto RegVal, auto ActualVal)
                {
                    return RendorMathVariant{RegVal * ActualVal};
                }, Num, Val);
    }
    
    /* ----------------------------------- div ---------------------------------- */
    void operator/(RendorMathVariant Val)
    {
        Num = std::visit([&](auto RegVal, auto ActualVal)
                {
                    return RendorMathVariant{RegVal / ActualVal};
                }, Num, Val);
    }
};

#endif // CONSTANT_HPP
#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <cstdint>
#include <string>
#include <map>

#include "Definitions.hpp"


struct Constant
{
    RendorVariant Const = 0;

    void operator=(RendorVariant value)
    {
        Const = std::move(value);
    }

    /* ----------------------------------- add ---------------------------------- */
    void operator+(RendorMathVariant Val)
    {
        switch (Const.index())
        {
            case 0:
            {
                auto ConstConverted = std::get_if<RendorInt>(&Const);
                Const = std::visit([&](RendorInt RegVal, auto ActualVal)
                {
                    return RendorVariant(RegVal + ActualVal);
                }, ConstConverted, Val);
                break;
            }
            case 1:
            {
                auto ConstConverted = std::get_if<RendorDouble>(&Const);
                Const = (*ConstConverted) - Int;
                break;
            }
        }
    }

    /* ----------------------------------- sub ---------------------------------- */
    void operator-(RendorMathVariant Int)
    {
        switch (Const.index())
        {
            case 0:
            {
                auto ConstConverted = std::get_if<RendorInt>(&Const);
                Const = (*ConstConverted) - Int;
                break;
            }
            case 1:
            {
                auto ConstConverted = std::get_if<RendorDouble>(&Const);
                Const = (*ConstConverted) - Int;
                break;
            }
        }
    }

    /* ----------------------------------- mul ---------------------------------- */
    void operator*(RendorMathVariant Int)
    {
        switch (Const.index())
        {
            case 0:
            {
                auto ConstConverted = std::get_if<RendorInt>(&Const);
                Const = (*ConstConverted) * Int;
                break;
            }
            case 1:
            {
                auto ConstConverted = std::get_if<RendorDouble>(&Const);
                Const = (*ConstConverted) * Int;
                break;
            }
        }
    }
    
    /* ----------------------------------- div ---------------------------------- */
    void operator/(RendorMathVariant Int)
    {
        switch (Const.index())
        {
            case 0:
            {
                auto ConstConverted = std::get_if<RendorInt>(&Const);
                Const = (*ConstConverted) / Int;
                break;
            }
            case 1:
            {
                auto ConstConverted = std::get_if<RendorDouble>(&Const);
                Const = (*ConstConverted) / Int;
                break;
            }
        }
    }
};

#endif // CONSTANT_HPP
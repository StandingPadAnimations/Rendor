#include "RendorInterpreter/Interpreter.hpp"


void Interpreter::MathVariantToConst(MathVariant& MathConst)
{
    switch (MathConst.index())
    {
        case 0:
        {
            std::visit([&](int64_t MathConstv) 
            { 
                DropInt64OnStack(MathConstv);
            }, MathConst);
            break;
        }
        case 1:
        {
            std::visit([&](double MathConstv) 
            { 
                DropDoubleOnStack(MathConstv);
            }, MathConst);
            break;
        }
    }
}
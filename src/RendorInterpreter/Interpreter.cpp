#include "RendorInterpreter/Interpreter.hpp"

#include <boost/range/detail/common.hpp>
#include <type_traits>

void Interpreter::ExecuteByteCode()
{
    for (const auto& Operation : CurrentModule->Main.FunctionOperations)
    {
        switch (Operation.Operation)
        {
            
            default:
            {
                throw error::RendorException("Unsupported instruction!");
            }
        }
    }
}
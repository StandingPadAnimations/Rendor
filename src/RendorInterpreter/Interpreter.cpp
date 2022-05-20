#include <bit>

#include "RendorInterpreter/Interpreter.hpp"

#include <boost/range/detail/common.hpp>
#include <type_traits>

void Interpreter::ExecuteByteCode()
{
    ByteCodeEnum ByteCodeOp;
    const auto Header = [](binary_io::any_istream a_stream) {
		
    };
}
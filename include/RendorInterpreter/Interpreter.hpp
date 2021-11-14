#ifndef INTERPRETER
#define INTERPRETER
#include <string>
#include <memory>
#include <fstream>
#include <map>
#include <algorithm>

#include "Exceptions.hpp"

// Other Parts of the Rendor Engine Interpreter 
#include "RendorInterpreter/MathEvaluator.hpp"
#include "RendorInterpreter/Rendor.hpp"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/finder.hpp>

namespace engineinterpreter{
    void ExecuteByteCode(std::ifstream& File);
}


#endif // INTERPRETER

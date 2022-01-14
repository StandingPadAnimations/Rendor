#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP
#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <map>
#include <memory>
#include <cmath>

#include "Exceptions.hpp"
#include "RendorInterpreter/RendorTypes.hpp"
#include "RendorInterpreter/VariableType.hpp"

#include <boost/tokenizer.hpp>
#include <boost/format.hpp>

std::string PostFixEval(std::string_view PostFixOperation, std::map<std::string, std::unique_ptr<Variable>> *Variables = NULL);

#endif // MATH_EVAL_HPP
#ifndef MATHEVAL
#define MATHEVAL
#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <map>
#include <memory>
#include <cmath>

#include "Exceptions.hpp"
#include "RendorInterpreter/Rendor.hpp"

#include <boost/tokenizer.hpp>

std::string EvalNums(std::string Num1, std::string Num2, std::string Op);
std::string PostFixEval(std::string_view PostFixOperation, std::map<std::string, std::unique_ptr<Variable>>& Variables);

#endif // MATHEVAL
#ifndef MATHEVAL
#define MATHEVAL
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <cmath>

#include "Exceptions.hpp"

#include <boost/tokenizer.hpp>

std::string EvalNums(std::string Num1, std::string Num2, std::string Op);
std::string PostFixEval(std::string_view PostFixOperation);

#endif // MATHEVAL
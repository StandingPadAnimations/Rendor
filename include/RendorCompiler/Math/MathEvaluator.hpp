#ifndef MATHEVAL
#define MATHEVAL
#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <map>
#include <cmath>

#include "Exceptions.hpp"

#include <boost/tokenizer.hpp>
#include <boost/format.hpp>

/*
This version of the header is specifically designed for the compiler
*/
std::string PostFixEval(std::string_view PostFixOperation);

#endif // MATHEVAL
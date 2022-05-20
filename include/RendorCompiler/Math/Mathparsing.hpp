#ifndef MATHPARSING
#define MATHPARSING
#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <map>
#include <stack>

#include "Exceptions.hpp"

#include <boost/tokenizer.hpp>

std::string OperationToPostfix(std::string_view Operation);

#endif // MATHPARSING
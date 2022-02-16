#ifndef MATHPARSING_HPP
#define MATHPARSING_HPP
#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <map>
#include <unordered_map>
#include <stack>

using std::string;
using std::string_view;
using std::list;
using std::map;
using std::unordered_map;
using std::stack;

#include "Exceptions.hpp"

#include <boost/tokenizer.hpp>

std::string OperationToPostfix(std::string_view Operation);

#endif // MATHPARSING_HPP
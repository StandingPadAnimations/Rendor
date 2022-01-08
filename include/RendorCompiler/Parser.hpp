#ifndef PARSER
#define PARSER
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "RendorCompiler/Lexer.hpp"
#include "RendorCompiler/Mathparsing.hpp"
#include "RendorCompiler/MathEvaluator.hpp"
#include "RendorCompiler/RendorDeltaOptimizer.hpp"
#include "RendorCompiler/Nodes.hpp"
#include "Exceptions.hpp"

#include <boost/format.hpp>

using lt = Lex::Token;

std::vector<std::string> Parser(const std::vector<std::pair<Lex::Token, std::string>>& Tokens);

#endif // * PARSER
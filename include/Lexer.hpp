#pragma once

// Includes everything needed for the Lexer class.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>
#include <sstream>
#include "PythonStatements.hpp"
#include "Exceptions.hpp"

// Creates a class called Lexer in the namespace Lex(to prevent conflicts).
namespace Lex{
    // Enum for tokens 
    enum class Token{
        Import,
        PreProcessDefine,
        Comment,
        Int,
        Float,
        Char,
        String,
        Bool,
        Paren,
        Bracket,
        Operator,
        Keyword,
        Variable,
    };

    enum class ID{
        None,
        Comment,
        Char,
        StringEnd, // Exists for error handling
        KeywordArgs,
        ParenArgs
    };

    // Lexer Class 
    class Lexer{
        private:
            // Python recreated functions for the Lexer
            py::Python LexPython;

            // Keywords to check for 
            std::string Keywords[4] {"edef", "while", "if", "return"};
            
        public:            
            // Tokenizes Lines of code from Rendor Files
            std::vector<std::pair<Token, std::string_view>> Tokenize(std::string&);
    };
}
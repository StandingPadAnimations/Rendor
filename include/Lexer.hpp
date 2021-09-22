#pragma once

// Includes everything needed for the Lexer class.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>
#include <sstream>
#include "Exceptions.hpp"

namespace Lex{
    // Enum for tokens 
    enum class Token{
        Import, // 0
        PreProcessDefine, // 1
        Comment, // 2
        Int, // 3 
        Float, // 4
        String, // 5
        Bool, // 6
        Paren, // 7
        Bracket, // 8
        Operator, // 9
        Keyword, // 10
        Variable, // 11
        FunctionCall, // 12
        Arithmic, // 13
        Increment, // 14
        Decrement, // 15
        ComparisonObject // 16
    };

    enum class ID{
        None,
        Comment,
        Char,
        KeywordArgs,
    };

    enum class SpecificID{
        None,
        CharDouble,
        CharSingle,
        CharTilda,
        ForwardSlash,
        EqualOperator,
        IncrementDecrementCheck
    };

    // Lexer Class 
    class Lexer{
        private:
            // Keywords to check for 
            std::string Keywords[5] {"edef", "while", "if", "return", "echo"};

            // Keywords to check for 
            std::string Operators[3] {"and", "or", "not"};
            
        public:            
            // Tokenizes Lines of code from Rendor Files
            std::vector<std::pair<Token, std::string>> Tokenize(std::string&);
    };
}
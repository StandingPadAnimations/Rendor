#pragma once

// Includes everything needed for the Lexer class.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
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
        ComparisonObject, // 16
        EntryFunction, // 17
        EndOfProgram, // 18
        ArgumentObject // 19
    };

    enum class ID{
        None,
        Comment,
        Char,
        KeywordArgs,
        Number
    };

    enum class SpecificID{
        None,
        CharDouble,
        CharSingle,
        CharTilda,
        ForwardSlash,
        EqualOperator,
        IncrementDecrementCheck,
        Increment,
        Decrement,
        Arithmic,
        Int,
        Float,
    };

    // Lexer Class 
    class Lexer{
        private:
            // Keywords to check for 
            std::string Keywords[6] {"edef", "while", "if", "return", "echo", "input"};

            // Keywords to check for 
            std::string Operators[4] {"and", "or", "not", "is"};
            
        public:            
            // Tokenizes Lines of code from Rendor Files
            std::vector<std::pair<Token, std::string>> Tokenize(std::string&);
    };
}
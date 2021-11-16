#ifndef LEXER
#define LEXER
// Includes everything needed for the Lexer class.
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <sstream>
#include <functional>
#include <string_view>

#include "Exceptions.hpp"

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace Lex{
    // Enum for tokens 
    enum class Token{
        // Identifiers, keywords, and functions
        IDENTIFIER,
        BUILT_IN_FUNCTION,
        KEYWORD,

        // Symbols
        EQUAL,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        NEWLINE,

        // Variable Types
        INT,
        FLOAT
    };

    // Lexer Class 
    class Lexer{
        public:            
            // Tokenizes Lines of code from Rendor Files
            std::vector<std::pair<Token, std::string>> Tokenize(const std::string&, std::string_view);

            // Contains paths of imports to compile later 
            std::vector<boost::filesystem::path> Imports;

            // CompileVariable; Lexer changes behavior when in Cpp compile mode
            bool LexerCompileMode = false;

            // Constructer
            explicit Lexer(const bool& CompileMode){
                LexerCompileMode = CompileMode;
            };
            
        private:
            // Keywords to check for 
            std::array<std::string, 5> Keywords {"while", "if", "return", "edef", "#rdef"};

            std::array<std::string, 2> Functions {"echo", "input"};

            // Operators to check for 
            std::array<std::string, 8> Operators {"and", "or", "not", "is", "==", "!=", ">=", "<="};
    };
}
#endif // * LEXER
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

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace Lex{
    // Enum for tokens 
    enum class Token{
        // Identifiers, keywords, and functions
        IDENTIFIER = 0, // Note: While values in enums don't need to be assigned, it makes reading the token output easier
        BUILT_IN_FUNCTION = 1,
        KEYWORD = 2,

        // Symbols
        EQUAL = 3,
        LPAREN = 4,
        RPAREN = 5,
        LBRACE = 6,
        RBRACE = 7,
        NEWLINE = 8, 
        COMMA = 9,

        // Variable Types
        INT = 10,
        FLOAT = 11,
        STRING = 12,
        BOOL = 13
    };

    // Lexer Class 
    class Lexer{
        public:            
            // Tokenizes Lines of code from Rendor Files
            std::vector<std::pair<Token, std::string>> Tokenize(const std::string&, std::string_view);

            // Contains paths of imports to compile later 
            std::vector<boost::filesystem::path> Imports;
            
        private:
            // Keywords to check for 
            std::array<std::string, 5> Keywords {"while", "if", "return", "edef", "#rdef"};

            std::array<std::string, 2> Functions {"echo", "input"};

            // Operators to check for 
            std::array<std::string, 8> Operators {"and", "or", "not", "is", "==", "!=", ">=", "<="};

            enum class BufferID 
            {
                None,
                StringEnd,
                CharSingle,
                CharDouble,
                CharTilda,
                Comment
            };
    };
}
#endif // * LEXER
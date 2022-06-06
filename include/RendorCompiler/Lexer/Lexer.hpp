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
#include <map>

#include "RendorBoostFileMapper.hpp"
#include "Exceptions.hpp"

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#define FALLTHROUGH [[fallthrough]]

namespace Lex{
    // Enum for tokens 
    enum class Token{
        // Identifiers, keywords, and functions
        IDENTIFIER, 
        KEYWORD,
        TYPE_HINT,
        ATTRIBUTE,
        OPERATOR,

        // Symbols
        EQUAL,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        NEWLINE, 
        COMMA,
        SEMICOLON,
        BIOP,
        UnOp,

        // Variable Types
        INT,
        FLOAT,
        STRING,
        BOOL
    };

    // Lexer Class 
    class Lexer{
        public:            
            // Tokenizes Lines of code from Rendor Files
            std::vector<std::pair<Token, std::string>> Tokenize(const boost::interprocess::mapped_region&);
            
        private:
            // Keywords to check for 
            std::vector<std::string_view> Keywords {"while", "if", "return", "edef", "else", "~forward", "cimport", "namespace", "extern", "export"};
            std::vector<std::string_view> Attributes {"~module_destroy"};
            std::vector<std::string_view> Types {"i64", "double", "string", "bool", "void"};

            // Operators to check for 
            std::array<std::string_view, 4> Operators {"and", "or", "not", "is"};

            std::map<std::string_view, char, std::less<>> BiOpTokens   {{{"="}, '='},
                                                                        {{">"}, '='},
                                                                        {{"<"}, '='},
                                                                        {{"!"}, '='},
                                                                        {{":"}, ':'},
                                                                        {{"-"}, '>'}};

            std::map<std::string_view, char, std::less<>> UnOpTokens   {{{"+"}, '+'},
                                                                        {{"-"}, '-'}};

            enum class BufferID 
            {
                None,
                StringEnd,
                CharSingle,
                CharDouble,
                CharTilda,
                Comment
            };

            bool LexerCharCheck(char Char);
    };
}
#endif // * LEXER
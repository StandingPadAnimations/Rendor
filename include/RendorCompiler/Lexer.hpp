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
        BUILT_IN_FUNCTION,
        KEYWORD,
        OPERATOR,

        // Symbols
        EQUAL,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        NEWLINE, 
        COMMA,
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
            std::vector<std::pair<Token, std::string>> Tokenize(const std::string&);

            // Contains paths of imports to compile later 
            std::vector<boost::filesystem::path> Imports;
            
        private:
            // Keywords to check for 
            std::array<std::string, 5> Keywords {"while", "if", "return", "edef", "else"};

            // Functions
            std::array<std::string, 3> Functions {"echo", "input", "sum"};

            // Operators to check for 
            std::array<std::string, 4> Operators {"and", "or", "not", "is"};

            std::map<std::string, char> BiOpTokens {{{"="}, '='},
                                                    {{">"}, '='},
                                                    {{"<"}, '='},
                                                    {{"!"}, '='}};

            std::map<std::string, char> UnOpTokens {{{"+"}, '+'},
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
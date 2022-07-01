#ifndef RENDOR_ASSEMBLER_LEXER_HPP
#define RENDOR_ASSEMBLER_LEXER_HPP
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

namespace LexAsm
{
    // Enum for tokens 
    enum class Token
    {
        LABEL,
        SYMBOL,
        NEWLINE,

        INT,
        FLOAT,
        STRING,
        BOOL,
    };

    // Lexer Class 
    class Lexer
    {
        public:            
            // Tokenizes Lines of code from Rendor Files
            std::vector<std::pair<Token, std::string>> Tokenize(const boost::interprocess::mapped_region&) const noexcept;
            
        private:
            const std::vector<std::string_view> Symbols {"$", "l", "g", "b"};

            // Operators to check for 
            const std::array<std::string_view, 4> Operators {"and", "or", "not", "is"};
            enum class BufferID 
            {
                None,
                StringEnd,
                CharSingle,
                CharDouble,
                CharTilda,
                Comment
            };

            bool LexerCharCheck(const char Char) const noexcept;
    };
}
#endif // RENDOR_ASSEMBLER_LEXER_HPP
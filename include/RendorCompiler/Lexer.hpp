#ifndef LEXER
#define LEXER
// Includes everything needed for the Lexer class.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include "Exceptions.hpp"

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

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
        ComparisonObject, // 13
        EntryFunction, // 14
        EndOfProgram, // 15
        ArgumentObjects, // 16
        Bop, // 17
        BopVariableRef, // 18
        Increment, // 19
        Decrement, // 20
        CppCompileTypeHint, // 21
        VariableReference, // 22
        NewLine // 23
    };


    // Lexer Class 
    class Lexer{
        public:            
            // * Tokenizes Lines of code from Rendor Files
            std::vector<std::pair<Token, std::string>> Tokenize(const std::string&, std::string_view);

            // * Contains paths of imports to compile later 
            std::vector<boost::filesystem::path> Imports;

            // * CompileVariable; Lexer changes behavior when in Cpp compile mode
            bool LexerCompileMode = false;

            // * Constructer
            explicit Lexer(const bool& CompileMode){
                LexerCompileMode = CompileMode;
            };
    };
}
#endif // * LEXER
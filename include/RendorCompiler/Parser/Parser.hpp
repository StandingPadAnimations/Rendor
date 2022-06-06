#ifndef RENDOR_PARSER_HPP
#define RENDOR_PARSER_HPP
#include <string>
#include <string_view>
#include <vector>

class RendorParser
{
    public:
        void GenerateAST();
        explicit RendorParser(std::vector<std::string>* input_Tokens) : Tokens(input_Tokens) {}
    
    private:
        std::size_t Index = 0;
        std::vector<std::string>* Tokens;
        std::string_view CurrentToken;
        void GetNextTok()
        {
            CurrentToken = (*Tokens)[Index];
            ++Index;
        };
};

#endif // RENDOR_PARSER_HPP
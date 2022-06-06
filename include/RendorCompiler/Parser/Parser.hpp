#ifndef RENDOR_PARSER_HPP
#define RENDOR_PARSER_HPP
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <map>

#include "RendorCompiler/Lexer/Lexer.hpp"
#include "RendorCompiler/Nodes/Nodes.hpp"
using LexTok = Lex::Token;
class RendorParser
{
    public:
        Main Main;
        void PrimaryParse();
        explicit RendorParser(std::vector<std::pair<LexTok, std::string>>* input_Tokens) : Tokens(input_Tokens), Scope(&Main.GlobalScope.Nodes)
        {
            GetNextTok(); // get the first token
            it = Tokens->begin();
            ++it;
        }
    
    private:
        std::size_t CurrentLine = 1;
        std::vector<std::pair<LexTok, std::string>>* Tokens;
        std::vector<std::pair<LexTok, std::string>>::iterator it = Tokens->begin();
        std::vector<std::vector<std::unique_ptr<nodes::Node>>*> ScopeList;
        std::vector<std::unique_ptr<nodes::Node>>* Scope;

        /* ---------------------------------- Types --------------------------------- */
        const std::map<std::string_view, IR_Type, std::less<>> Str_to_Type 
        {
            {"void",   IR_Type::VOID},
            {"i64",    IR_Type::INT},
            {"double", IR_Type::DOUBLE},
            {"string", IR_Type::STRING},
            {"bool",   IR_Type::BOOL},
        };

        /* -------------------------- Token and value pair -------------------------- */
        LexTok CurrentToken{};
        std::string_view CurrentValue{};

        /* -------------------------------- Functions ------------------------------- */
        void GetNextTok()
        {
            std::tie(CurrentToken, CurrentValue) = *it;
            ++it;
        };

        void AddToMain(std::unique_ptr<nodes::Node> Node)
        {
            Scope->push_back(std::move(Node));
        }

        void AddScope(std::vector<std::unique_ptr<nodes::Node>>* i_Scope)
        {
            ScopeList.push_back(Scope);
            Scope = i_Scope;
        }

        void RemoveScope()
        {
            Scope = ScopeList.back();
            ScopeList.pop_back();
        }

        void ParseIdentifer();
        void ParseEdef();
        ConstPtr ParseConstant();
};

#endif // RENDOR_PARSER_HPP
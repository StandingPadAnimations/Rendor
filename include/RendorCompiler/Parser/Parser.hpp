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

        std::map<std::string, RendorIR::IR_Type, std::less<>> Variables;

        /* ---------------------------------- Types --------------------------------- */
        const std::map<std::string_view, RendorIR::IR_Type, std::less<>> Str_to_Type 
        {
            {"void",   RendorIR::IR_Type::VOID},
            {"i64",    RendorIR::IR_Type::INT},
            {"double", RendorIR::IR_Type::DOUBLE},
            {"string", RendorIR::IR_Type::STRING},
            {"bool",   RendorIR::IR_Type::BOOL},
        };

        const std::map<RendorIR::IR_Type, std::string_view, std::less<>> Type_to_Str 
        {
            {RendorIR::IR_Type::VOID,   "void"},
            {RendorIR::IR_Type::INT,    "i64"},
            {RendorIR::IR_Type::DOUBLE, "double"},
            {RendorIR::IR_Type::STRING, "string"},
            {RendorIR::IR_Type::BOOL,   "bool"},
        };

        /* -------------------------- Token and value pair -------------------------- */
        LexTok CurrentToken{};
        std::string_view CurrentValue{};

        /* -------------------------------- Functions ------------------------------- */
        void GetNextTok()
        {
            do
            {
                if (it == Tokens->end())
                {
                    return;
                }
                std::tie(CurrentToken, CurrentValue) = *it;
                ++it;
                if (CurrentToken == LexTok::NEWLINE)
                {
                    ++CurrentLine;
                }
            } while (CurrentToken == LexTok::NEWLINE);
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

        void ParseIdentifer(const std::string& Identifier);
        void ParseEdef(const std::string& Identifier);
        void ParseImport(const std::string& Identifier);
        ConstPtr ParseConstant();
};

#endif // RENDOR_PARSER_HPP
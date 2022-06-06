#include "RendorCompiler/Parser/Parser.hpp"
#include "boost/lexical_cast.hpp"

ConstPtr RendorParser::ParseConstant()
{
    switch (CurrentToken)
    {
        case LexTok::INT:
        {
            auto Ret = std::make_unique<nodes::Int>();
            Ret->Val = boost::lexical_cast<std::int64_t>(std::string{CurrentValue});
            return Ret;
        }
        case LexTok::FLOAT:
        {
            auto Ret = std::make_unique<nodes::Double>();
            Ret->Val = stod(std::string{CurrentValue});
            return Ret;
        }
        case LexTok::STRING:
        {
            auto Ret = std::make_unique<nodes::String>();
            Ret->Val = std::string{CurrentValue};
            return Ret;
        }
        case LexTok::BOOL:
        {
            auto Ret = std::make_unique<nodes::Bool>();
            Ret->Val = CurrentValue == "true" ? true : false;
            return Ret;
        }
        case LexTok::IDENTIFIER:
        {
            auto Ret = std::make_unique<nodes::Reference>();
            Ret->Val = std::string{CurrentValue};
            return Ret;
        }
        default:
        {
            break;
        }
    }
    return nullptr;
}
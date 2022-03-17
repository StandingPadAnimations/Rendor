#include "RendorCompiler/Parser/Parser.hpp"

void Parser::PopTempID()
{
    ParserTempIDList.pop_back();
}
void Parser::AddTempID(TempID ID)
{
    ParserTempIDList.emplace_back(ID);
}
void Parser::PopScope()
{
    ScopeList.pop_back();
}
void Parser::AddScope(std::vector<std::unique_ptr<Node>>* Ptr)
{
    ScopeList.push_back(Ptr);
}
void Parser::PushToScope(std::unique_ptr<Node> Node)
{
    Scope->push_back(std::move(Node));
}

void Parser::ReplaceNode(std::unique_ptr<Node> Node)
{
    Scope->back() = std::move(Node);
}
NodeType Parser::GetTypeOfNode()
{
    return Scope->back()->Type;
}
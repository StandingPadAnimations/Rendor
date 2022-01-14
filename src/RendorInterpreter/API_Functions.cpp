#include "RendorInterpreter/Interpreter.hpp"

void Interpreter::MarkConstIDForDisposal(std::string_view RendorConstID)
{
    WhiteObjects.emplace_back(RendorConstID);
}
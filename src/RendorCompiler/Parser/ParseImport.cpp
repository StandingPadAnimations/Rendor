#include "RendorCompiler/Parser/Parser.hpp"

void RendorParser::ParseImport(const std::string& Identifier)
{
    if (CurrentToken != LexTok::STRING)
    {
        throw error::CompilerRendorException(fmt::format("Line {}: Expected string for import, got {} instead!", CurrentLine, CurrentValue), CurrentLine);
    }

    std::unique_ptr<nodes::Import> Import = std::make_unique<nodes::Import>();
    Import->Import = Identifier;
    Import->Type = nodes::ImportType::C_CPP_DLL; // TODO: Add a way to tell between C dlls and Rendor imports
    AddToMain(std::move(Import));
}
#include <filesystem>

#include "RendorCompiler/Lexer/Lexer.hpp"
#include "RendorCompiler/Parser/Parser.hpp"
#include "RendorEngine.hpp"

#include <fmt/color.h>

namespace fs = std::filesystem;
namespace bi = boost::interprocess;

// Overload of new for debugging
#if DEBUGMODE
void* operator new(size_t size)
{
    fmt::print("Allocating {} bytes\n", size);
    return malloc(size);
}
#endif

void RendorEngineCompiler::run(const std::string& FileInput, const std::vector<std::string_view>& Arguments)
{
    if (!fs::exists(FileInput))
    {
        throw error::RendorException(fmt::format("{} doesn't exist!", FileInput));
    }

    const fs::path AbsPath(FileInput);
    const std::string AbsPathExt = AbsPath.extension().string();
    std::ios_base::sync_with_stdio(false);
    std::string OutputPath = AbsPath.parent_path().string();

    // * Checks for seeing if the file is compatible with the interpreter
    if (AbsPathExt != ".ren")
    {
        throw error::RendorException("Fatal Error; Missing Rendor File input");
    } 


    if (Arguments.size() >= 2) // * for all arguments other then the file 
    { 
        if
        ((std::find(Arguments.begin(), Arguments.end(), "-d") != Arguments.end()) ||
        (std::find(Arguments.begin(), Arguments.end(), "-debug") != Arguments.end()))
        {
            DebugMode = true;
        }

        if
        ((std::find(Arguments.begin(), Arguments.end(), "-o") != Arguments.end()) ||
        (std::find(Arguments.begin(), Arguments.end(), "-output") != Arguments.end()))
        {
            auto Arg = std::find(Arguments.begin(), Arguments.end(), "-o");
            ++Arg;
            OutputPath = *Arg;
        }

        if
        ((std::find(Arguments.begin(), Arguments.end(), "-we") != Arguments.end()) ||
        (std::find(Arguments.begin(), Arguments.end(), "-warn-errors") != Arguments.end()))
        {
            WarningsToErrors = true;
        }

        if(std::find(Arguments.begin(), Arguments.end(), "-O0") != Arguments.end())
        {
            OptimizeByteCode = false;
        }

        if(std::find(Arguments.begin(), Arguments.end(), "-O1") != Arguments.end())
        {
            if (!OptimizeByteCode)
            {
                throw error::RendorException("Can not enable and disable optimization at the same time!");
            }
        }
    }


    fmt::print(fg(fmt::color::green), "Tokenizing...\n");
    const bi::file_mapping MappedFile = bi::file_mapping(FileInput.c_str(), bi::read_only);
    const bi::mapped_region MappedRendorFileMemory = bi::mapped_region(MappedFile, bi::read_only);
    const Lex::Lexer RenLexer;
    const std::vector<std::pair<Lex::Token, std::string>> Tokens = RenLexer.Tokenize(RendorFileMemory); // Tokenizes code for parser

    // Parses
    fmt::print(fg(fmt::color::green), "Generating AST tree......\n");
    RendorParser Parser{&Tokens};
    Parser.PrimaryParse();

    //     fmt::print(fg(fmt::color::green), "Generating bytecode.........\n");
    //     for (const auto& Node : (*Parser::Script.GlobalBody))
    //     {
    //         if (Node)
    //         {
    //             RenASTInspector.InspectAST(Node);
    //             Node->CodeGen();
    //         }
    //     }
    

    // fmt::print(fg(fmt::color::green), "Outputing bytecode...........\n");
    // std::string AbsPathCrenOutput = "/" + AbsPath.filename().replace_extension(".Cren").string();
    // Parser::Script.CompileByteCode(OutputPath + AbsPathCrenOutput);

    if (DebugMode)
    { 
        fmt::print(fg(fmt::color::green), "----------------------------TOKENS----------------------------\n");
        for (const auto& [token, value] : Tokens)
        {
            fmt::print(fg(fmt::color::green), "Token: {} {}\n", static_cast<std::underlying_type<Lex::Token>::type>(token), value);
        }
        fmt::print(fg(fmt::color::green), "----------------------------AST----------------------------\n");
        Parser.Main.PrintTree();
    }
}
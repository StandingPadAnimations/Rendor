#include "RendorAssembler/Lexer.hpp"
#include "RendorBoostFileMapper.hpp"
#include "RendorEngine.hpp"
#include <filesystem>
#include <fmt/format.h>

namespace fs = std::filesystem;
namespace bi = boost::interprocess;
void RendorEngineAssembler::run(const std::string& FileInput, const std::vector<std::string_view> &Arguments)
{
    if (!fs::exists(FileInput))
    {
        throw error::RendorException(fmt::format("{} doesn't exist!", FileInput));
    }
    const fs::path AbsPath(FileInput);
    const std::string AbsPathExt = AbsPath.extension().string();
    std::ios_base::sync_with_stdio(false);
    std::string OutputPath = AbsPath.parent_path().string();

    if (AbsPathExt != ".ren_asm")
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
    }


    fmt::print(fg(fmt::color::green), "Tokenizing...\n");
    const bi::file_mapping MappedFile = bi::file_mapping(FileInput.c_str(), bi::read_only);
    const bi::mapped_region MappedRendorFileMemory = bi::mapped_region(MappedFile, bi::read_only);
    const LexAsm::Lexer RenLexer;
    const std::vector<std::pair<LexAsm::Token, std::string>> Tokens = RenLexer.Tokenize(RendorFileMemory); // Tokenizes code for parser

    if (DebugMode)
    { 
        fmt::print(fg(fmt::color::green), "----------------------------TOKENS----------------------------\n");
        for (const auto& [token, value] : Tokens)
        {
            fmt::print(fg(fmt::color::green), "Token: {} {}\n", static_cast<std::underlying_type<LexAsm::Token>::type>(token), value);
        }
    }
}
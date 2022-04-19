#include "RendorEngine.hpp"
#include "RendorCompiler/Lexer/Lexer.hpp"
#include "RendorCompiler/Parser/Parser.hpp"
#include <fmt/color.h>

// Overload of new for debugging
#if DEBUGMODE
void* operator new(size_t size)
{
    fmt::print("Allocating {} bytes\n", size);
    return malloc(size);
}
#endif

void RendorEngineCompiler::run(const std::string& FileInput, std::vector<std::string_view>& Arguments)
{
    // * Boost variables for checking some stuff
    // ? Personally I think there may be a way to use less variables 
    fs::path AbsPath(FileInput);
    std::string AbsPathExt = AbsPath.extension().string();
    std::ios_base::sync_with_stdio(false);
    OutputPath = AbsPath.parent_path().string();

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

        if
        ((std::find(Arguments.begin(), Arguments.end(), "-c") != Arguments.end()) ||
        (std::find(Arguments.begin(), Arguments.end(), "-cpp") != Arguments.end()))
        {
            fmt::print(fg(fmt::color::red), "C++ transpiling not supported in this version\n");
        }
    }

    {
        boost::interprocess::file_mapping File(FileInput.c_str(), boost::interprocess::read_only);
        boost::interprocess::mapped_region RendorFileMemory(File, boost::interprocess::read_only);

        // Tokenizes the AllCode string
        Lex::Lexer RenLexer;
        Parser RenParser;
        ASTInspector RenASTInspector;
        std::vector<std::pair<Lex::Token, std::string>> Tokens;

        fmt::print(fg(fmt::color::green), "Tokenizing...\n");
        Tokens = RenLexer.Tokenize(RendorFileMemory); // Tokenizes code for parser 

        // Parses
        fmt::print(fg(fmt::color::green), "Generating AST tree......\n");
        RenParser.ASTGeneration(Tokens); 

        fmt::print(fg(fmt::color::green), "Generating bytecode.........\n");
        for (const auto& Node : (*Parser::Script.GlobalBody))
        {
            RenASTInspector.InspectAST(Node);
            Node->CodeGen();
        }

        // Adds it to output Cren File
        {
            fmt::print(fg(fmt::color::green), "Outputing bytecode...........\n");
            std::string AbsPathCrenOutput = "/" + AbsPath.filename().replace_extension(".Cren").string();
            std::ofstream CrenOutput(OutputPath + AbsPathCrenOutput);
            for (auto const& Op : RendorEngineCompiler::ByteCode)
            {
                CrenOutput << Op << "\n";
            }
        }
    
        if (DebugMode)
        { 
            fmt::print(fg(fmt::color::green), "----------------------------DEBUG MODE----------------------------\n");
            for (auto const& [token, value] : Tokens)
            {
                fmt::print(fg(fmt::color::green), "Token: {} {}\n", static_cast<std::underlying_type<Lex::Token>::type>(token), value);
            }
            fmt::print("\n");
            for (auto const& command : RendorEngineCompiler::ByteCode)
            {
                fmt::print(fg(fmt::color::green), "{}\n", command);
            }
        }
    }
}
#include "RendorEngine.hpp"
#include "RendorCompiler/Lexer.hpp"
#include "RendorCompiler/Parser.hpp"
#include "cpp-terminal/base.hpp"

using Term::color;
using Term::fg;
using Term::bg;
using Term::style;

void RendorEngineCompiler::run (const std::string& FileInput, std::vector<std::string>& Arguments)
{
    // * Boost variables for checking some stuff
    // ? Personally I think there may be a way to use less variables 
    bfs::path AbsPath(FileInput);
    std::string AbsPathExt = AbsPath.extension().string();
    std::string AbsPathParentDir = AbsPath.parent_path().string();
    std::string AbsPathRenCache = AbsPathParentDir + "/.__rencache__";
    std::ios_base::sync_with_stdio(false);

    // * Checks for seeing if the file is compatible with the interpreter
    if (AbsPathExt != ".ren")
    {
        std::string ErrorMessage = (boost::format("%s%sFatal Error; %sMissing Rendor File input") % color(fg::red) % color(style::bold) % color(fg::reset)).str();
        throw error::RendorException(ErrorMessage);
    } 

    if (!bfs::is_directory(AbsPathRenCache)) // creates .__rencache__ folder if it doesn't exist
    { 
        bfs::create_directory(AbsPathRenCache);
    }

    // * Variables for command line arguments
    bool DebugMode = false;

    if (Arguments.size() >= 2) // * for all arguments other then the file 
    { 
        if
        ((std::find(Arguments.begin(), Arguments.end(), "-d") != Arguments.end()) ||
        (std::find(Arguments.begin(), Arguments.end(), "-debug") != Arguments.end()))
        {
            DebugMode = true;
        }

        if
        ((std::find(Arguments.begin(), Arguments.end(), "-c") != Arguments.end()) ||
        (std::find(Arguments.begin(), Arguments.end(), "-cpp") != Arguments.end()))
        {
            std::cout << color(fg::red) << "C++ transpiling not supported in this version" << color(style::reset) << std::endl;
        }
    }

    std::ifstream File(FileInput);
    std::vector<std::string> ByteCode;
    std::string AllCode;
    
    {
        for (std::string PreProcessLine; std::getline(File, PreProcessLine);)
        {
            boost::algorithm::trim(PreProcessLine);
            AllCode += PreProcessLine + ";";
        }


        // Tokenizes the AllCode string
        Lex::Lexer RenLexer;
        std::vector<std::pair<Lex::Token, std::string>> Tokens;

        std::cout << color(fg::green) << "Tokenizing..." << std::endl;
        Tokens = RenLexer.Tokenize(AllCode); // Tokenizes code for parser 

        // Parses
        std::cout << color(fg::green) << "Generating AST tree..." << std::endl;
        ByteCode = Parser(Tokens); 

        // Adds it to output Cren File
        if (ByteCode.size() > 0)
        {
            std::cout << color(fg::green) << "Outputing bytecode..." << std::endl;
            std::string AbsPathCrenOutput = "/" + AbsPath.filename().replace_extension(".Cren").string();
            std::ofstream CrenOutput(AbsPathRenCache + AbsPathCrenOutput);
            for (auto const& Op : ByteCode)
            {
                CrenOutput << Op << std::endl;
            }
        }
    
        if (DebugMode)
        { 
            std::cout << color(fg::green) << "----------------------------DEBUG MODE----------------------------" << std::endl;
            for (auto const& [token, value] : Tokens)
            {
                std::cout << color(fg::green) << "Token: " << static_cast<std::underlying_type<Lex::Token>::type>(token) << " " << value << std::endl;
            }
            std::cout << " " << std::endl;
            for (auto const& command : ByteCode)
            {
                std::cout << color(fg::green) << command << std::endl;
            }
        }
        std::cout << color(fg::reset) << std::endl;
    }
}
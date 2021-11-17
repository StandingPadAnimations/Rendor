#include "RendorEngine.hpp"

void RendorEngineCompiler::run (const std::string FileInput, char *argv[])
{
    

    // * Boost variables for checking some stuff
    // ? Personally I think there may be a way to use less variables 
    bfs::path AbsPath(FileInput);
    std::string AbsPathExt = AbsPath.extension().string();
    std::string AbsPathParentDir = AbsPath.parent_path().string();
    std::string AbsPathRenCache = AbsPathParentDir + "/.__rencache__";

    // * Checks for seeing if the file is compatible with the interpreter
    if (AbsPathExt != ".ren")
    {
        throw error::RendorException("Rendor only allows .ren files");
    } 

    if (!bfs::is_directory(AbsPathRenCache)) // creates .__rencache__ folder if it doesn't exist
    { 
        bfs::create_directory(AbsPathRenCache);
    }

    // * Variables for command line arguments
    bool DebugMode = false;

    if (argv[2] != NULL) // * for all arguments other then the file 
    { 
        if(
        (std::string(argv[2]) == "-debug") ||
        (std::string(argv[2]) == "-d"))
        {
            DebugMode = true;
        }

        else if (std::string(argv[2]) == "-cpp")
        {
            if(
            (std::string(argv[3]) == "-debug") ||
            (std::string(argv[3]) == "-d"))
            {
                DebugMode = true;
            }
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
        Tokens = RenLexer.Tokenize(AllCode, AbsPathParentDir); // Tokenizes code for parser 

        // Parses
        // ByteCode = Parser(Tokens); 

        // // Adds it to output Cren File
        // std::string AbsPathCrenOutput = "/" + AbsPath.filename().replace_extension(".Cren").string();
        // std::ofstream CrenOutput(AbsPathRenCache + AbsPathCrenOutput);
        for (auto const& [token, value] : Tokens)
        {
            std::cout << "Token: " << static_cast<std::underlying_type<Lex::Token>::type>(token) << " " << value << std::endl;
        }
    }
    if (DebugMode)
    { 
        std::cout << "----------------------------DEBUG MODE----------------------------" << std::endl;
        // for (auto const& command : ByteCode)
        // {
        //     std::cout << command << std::endl;
        // }
    }
    File.close();
}
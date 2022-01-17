#include "RendorEngine.hpp"
#include "RendorInterpreter/Interpreter.hpp"

void RendorEngineInterpreter::run (const std::string FilePath)
{
    bfs::path AbsPath(FilePath);
    std::string AbsPathExt = AbsPath.extension().string();

    {
        if (AbsPathExt == ".ren") // if the file is a .ren file 
        {
            throw error::RendorException(".Cren files only!");
        } 

        else if (AbsPathExt == ".Cren") // if the file is a compiled .ren file 
        {
            if (!bfs::exists(AbsPath)) 
            {
                throw error::RendorException((boost::format("There either is no .Cren file of %s; Compile your code first") % FilePath).str());
            }
        }

        else 
        {
            throw error::RendorException("rendor only allows .Cren files");
        } 
    }

    // Open file and execute code 
    std::ifstream File(AbsPath);

    if (File.is_open()) 
    {
        Interpreter::ExecuteByteCode(File);
    } 
    else 
    {
        throw error::RendorException("File is not open");
    }
}
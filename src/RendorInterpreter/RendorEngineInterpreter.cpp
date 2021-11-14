#include "RendorEngine.hpp"

void RendorEngineInterpreter::run (const std::string FilePath)
{
    bfs::path AbsPath(FilePath);
    std::string AbsPathExt = AbsPath.extension().string();

    {
        if (AbsPathExt == ".ren") // if the file is a .ren file 
        {
            std::string AbsPathToCren = (boost::format("%s/.__rencache__/%s") % AbsPath.parent_path().string() % AbsPath.filename().replace_extension(".Cren").string()).str();

            if (!bfs::exists(AbsPathToCren)) // see if the compiled version exists 
            {
                throw error::RendorException((boost::format("There either is no .Cren file of %s; Compile your code first") % FilePath).str());
            }

            if (bfs::last_write_time(AbsPath) > bfs::last_write_time(AbsPathToCren)) // check if the compiled version is older then the original 
            {
                std::cout << (boost::format("%s was modified recently but has not yet been recompiled;\nWe all forget to recompile so don't slam your head\n") % AbsPath.filename().string()) << std::endl;
            }
            AbsPath = AbsPathToCren;
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
            throw error::RendorException("rendor only allows .ren files");
        } 
    }

    // Open file and execute code 
    std::ifstream File(AbsPath);

    if (File.is_open()) 
    {
        engineinterpreter::ExecuteByteCode(File);
    } 
    else 
    {
        throw error::RendorException("File is not open");
    }
}
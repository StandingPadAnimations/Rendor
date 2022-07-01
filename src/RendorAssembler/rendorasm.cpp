#include <cstdlib>

#include "RendorEngine.hpp"
#include "Exceptions.hpp"

int main(int argc, char *argv[])
{
    try
    {
        #if DEBUGMODE
        std::cin.get();
        #endif
        
        if (argc > 1) 
        {
            const std::vector<std::string_view> Arguments(argv, argv + argc);
            const std::string File = std::string{Arguments[1]};
            RendorEngineAssembler Assembler;
            Assembler.run(File, Arguments);
        }
        else 
        {
            throw error::RendorException("Missing File Input!");
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exp)
    {        
        #if DEBUGMODE
        std::cin.get();
        #endif     
        fmt::print(fg(fmt::color::red), "{}\n", exp.what());
        return EXIT_FAILURE;
    } 
}
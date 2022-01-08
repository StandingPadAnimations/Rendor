/*
---------------------------------
---------------------------------
PLEASE READ BEFORE CONTINUING 
---------------------------------
---------------------------------

This is the main file for Project Rendor's bytecode compiler by Mahid Sheikh(https://twitter.com/StandingPadYT), 
an updated version of the EnderKnight Programming Language by Mahid Sheikh.

This project is open sourced, as long as you give credit to Mahid Sheikh the main file of the source code.
To give credit, please share StandingPad's twitter link(and the original GitHub(https://github.com/StandingPadAnimations/EnderKnight-Programming-Language)). 
Once done, you may then credit yourself for any changes you make

Any modifications must follow GNU GPL V3

It's also good(though not required) to keep consistency with the main code when modifying.
It should also be good to have a part within this section to list the changes you made to the code.

Other then that, you may now take a look at the rest of the code.

and please excuse my terrible speeling lol
*/

/*
Symbol Key from the Better Comments Extension in VSCode 
While the extension isn't required, it does make the code more readable in my opinian 
*/
// * Important info in the Dev's opinian 
// ! Broken code or code so unreadable it's worthy of getting this
// TODO: Dev knows that this could be improved but currently isn't sure how/Reminder to do something
// ? Dev think this could be improved but isnt't sure how 
// Nothing is normal comments


// Includes files needed for the main file

#include <iostream>
#include "RendorEngine.hpp"
#include "cpp-terminal/base.hpp"

int main (int argc, char *argv[])
{
    try
    {
        std::string File = "._.";
        std::string RendorExecutable = argv[0]; 
        std::vector<std::string> Arguments;

        if (argc > 1) {
            Arguments.assign(argv + 1, argv + argc);
            if (argc >= 2)
            {
                File = Arguments[0];
            }
        }

        if (RendorExecutable != RENDOR_BINARY_NAME)
        {
            std::cout << Term::color(Term::fg::red) << "Dealing with renamed version of rendorc. Version is: " << RENDOR_VERSION << Term::color(Term::style::reset) << std::endl;
        }

        RendorEngineCompiler::run(File, Arguments);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exp)
    {             
        std::cout << exp.what() << Term::color(Term::style::reset) << std::endl;   
        return EXIT_FAILURE;
    }
    return 0;
}


/*
---------------------------------
---------------------------------
PLEASE READ BEFORE CONTINUING 
---------------------------------
---------------------------------

This is the main file for Project Rendor's interpreter by Mahid Sheikh(https://twitter.com/StandingPadYT), 
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

#include <iostream>
#include <fstream>
#include "RendorInterpreter/Interpreter.hpp"
#include "Exceptions.hpp"

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace Bfs = boost::filesystem;

int main(int argc, char *argv[]){
    try{
        Bfs::path AbsPath(argv[1]);
        std::string AbsPathExt = AbsPath.extension().string();
        if(AbsPathExt == ".ren"){
            std::string AbsPathToCren = (boost::format("%s/.__rencache__/%s") % AbsPath.parent_path().string() % AbsPath.filename().replace_extension(".Cren").string()).str();

            if(!Bfs::exists(AbsPathToCren)){
                throw error::RendorException((boost::format("There either is no .Cren file of %s; Compile your code first") % argv[1]).str());
            }

            std::ifstream File(AbsPathToCren);
            if(File.is_open()){
                ExecuteByteCode(File);
            } else {
                throw error::RendorException("File is not open");
            }
        } else{
            throw error::RendorException("rendor only allows .ren files");
        }
        return EXIT_SUCCESS;
    }
    catch(std::exception& exp){
        std::cout << exp.what() << std::endl;
        return EXIT_FAILURE;
    }
}
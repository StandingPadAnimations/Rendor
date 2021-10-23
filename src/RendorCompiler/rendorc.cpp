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
#include <fstream>
#include "RendorCompiler/Lexer.hpp"
#include "RendorCompiler/Parser.hpp"
#include "ExtraFunctions.hpp"

// Boost libraries and macros
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// TODO: Make this more readable 
// ! Make. It. Readable.
int main(int argc, char *argv[]){
    // Sets variables and initializes them.
    // std::ifstream checks the file argument(arg[1]) and then takes in the file contents.
    try{
        std::ifstream File(argv[1]);
        std::vector<std::string> ByteCode;
        std::string AllCode;

        // * Boost variables for checking some stuff
        // ? Personally I think there may be a way to use less variables 
        boost::filesystem::path AbsPath(argv[1]);
        std::string AbsPathExt = AbsPath.extension().string();
        std::string AbsPathParentDir = AbsPath.parent_path().string();
        std::string AbsPathRenCache = AbsPathParentDir + "/.__rencache__";

        // * Checks for seeing if the file is compatible with the interpreter
        if((AbsPathExt == ".ren") || (AbsPathExt == ".Cren")){
            if(AbsPathExt == ".Cren"){
                throw error::RendorException("Can't use .Cren files currently!");
            }
        } else{
            throw error::RendorException("Rendor only allows .ren and .Cren files");
        }

        if(!boost::filesystem::is_directory(AbsPathRenCache)){ // creates .__rencache__ folder if it doesn't exist
            boost::filesystem::create_directory(AbsPathRenCache);
        }

        // * Variables for command line arguments
        bool DebugMode = false;
        bool CompileCppMode = false;

        if(argv[2] != NULL){ // * for all arguments other then the file 
            if(
            (std::string(argv[2]) == "-debug") ||
            (std::string(argv[2]) == "-d")
            ){
                DebugMode = true;
            }

            else if((std::string(argv[2]) == "-cpp")){
                CompileCppMode = true;
                if(
                (std::string(argv[3]) == "-debug") ||
                (std::string(argv[3]) == "-d")
                ){
                    DebugMode = true;
                }
            }
        }
        
            {
                ex::Extra ExtraFunctions;
                for(std::string PreProcessLine; std::getline(File, PreProcessLine);){
                    AllCode = AllCode + ExtraFunctions.reduce(PreProcessLine, " ", " \t") + "\n";
                }
            }
            // Tokenizes the AllCode string
            {
                Lex::Lexer RenLexer(CompileCppMode);
                std::vector<std::pair<Lex::Token, std::string>> Tokens;
                Tokens = RenLexer.Tokenize(AllCode, AbsPathParentDir); // Tokenizes code for parser 

                // Parses
                ByteCode = Parser(Tokens);

                // Adds it to output Cren File
                std::string AbsPathCrenOutput = "/" + AbsPath.filename().replace_extension(".Cren").string();
                std::ofstream CrenOutput(AbsPathRenCache + AbsPathCrenOutput);
                for(auto const& command : ByteCode){
                    CrenOutput << command << std::endl;
                }
            }
            if(DebugMode){ // Creates the .Cren output 
                std::cout << "----------------------------DEBUG MODE----------------------------" << std::endl;
                for(auto const& command : ByteCode){
                    std::cout << command << std::endl;
                }
            }
        File.close();
        return EXIT_SUCCESS;
    }
    catch(const std::exception& exp){             
        std::cout << exp.what() << std::endl;   
        return EXIT_FAILURE;
    }
    catch(...){
        std::cout << "whoops" << std::endl;
    }
    return 0;
}


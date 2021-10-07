/*
---------------------------------
---------------------------------
PLEASE READ BEFORE CONTINUING 
---------------------------------
---------------------------------

This is the main file for Project Rendor(by StandingPad Animations(https://twitter.com/StandingPadYT)), 
an updated version of the EnderKnight Programming Language by StandingPad Animations.

This project is open sourced, as long as you give credit to StandingPad Animations the main file of the source code.
To give credit, please share StandingPad's twitter link(and the original GitHub(https://github.com/StandingPadAnimations/EnderKnight-Programming-Language)). 
Once done, you may then credit yourself for any changes you make

This may not be edited for malicious purposes, 
nor can you take the code without changing a big part of it(like adding new features, fixing bugs, etc) and claim it as a edit.
and you may also not make a closed source version of this software.
If you're still confused on what's allowed, you can refer to GNU GPL V3.

It's also good(though not required) to provide some comments within the code to make it easier for others to follow.
It should also be good to have a part within this section to list the changes you made to the code.

Other then that, you may now take a look at the rest of the code.

and please excuse my terrible speeling lol
*/



// Includes files needed for the main file
#include <iostream>
#include <fstream>
#include "Lexer.hpp"
#include "ExtraFunctions.hpp"
#include "Preprocessor.hpp"

// Boost libraries 
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>

int main(int argc, char *argv[]){
    // Sets variables and initializes them.
    // std::ifstream checks the file argument(arg[1]) and then takes in the file contents.
    try{
        std::vector<std::pair<Lex::Token, std::string>> Tokens;
        bool DebugMode = false;
        std::ifstream File(argv[1]);
        
        // Boost variables for checking some stuff
        boost::filesystem::path AbsPath(argv[1]);
        std::string AbsPathExt = AbsPath.extension().string();
        std::string AbspathParentDir = AbsPath.parent_path().string();

        if((AbsPathExt == ".ren") || (AbsPathExt == ".Cren")){
            if(AbsPathExt == ".Cren"){
                throw error::RendorException("Can't use .Cren files currently!");
            }
        }

        else{
            throw error::RendorException("Rendor only allows .ren and .Cren files");
        }

        if(!boost::filesystem::is_directory(AbspathParentDir + "/.__rencache__")){
            boost::filesystem::create_directory(AbspathParentDir + "/.__rencache__");
        }

        if(argv[2] != NULL){
            if(
            (std::string(argv[2]) == "-debug") ||
            (std::string(argv[2]) == "-d")
            ){
                DebugMode = true;
            }
            else if(
            (std::string(argv[2]) == "-compile") ||
            (std::string(argv[2]) == "-c")
            ){
                throw error::RendorException("Compiling not yet supported");
            }
        }
        
        { // Start of compiling scope(may be an if statement in the future)
            std::string AllCode = PreProcess(File);
            Lex::Lexer RenLexer;

            // Tokenizes the AllCode string
            Tokens = RenLexer.Tokenize(AllCode);

            if(DebugMode){
                std::ofstream CrenOutput(AbspathParentDir + "/.__rencache__" + "/" + AbsPath.filename().replace_extension(".Cren").string());
                for(auto const& [token, value] : Tokens){
                    CrenOutput << static_cast<std::underlying_type<Lex::ID>::type>(token) << " " << value << std::endl;
                }
            }
        } // End of scope

        File.close();
        return EXIT_SUCCESS;
    }
    catch(const std::exception& exp){             
        std::cout << exp.what() << std::endl;   
        return EXIT_FAILURE;
    }
    return 0;
}


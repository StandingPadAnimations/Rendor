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


int main(int argc, char *argv[]){
    // Sets variables and initializes them.
    // std::ifstream checks the file argument(arg[1]) and then takes in the file contents.
    try{
        std::vector<std::pair<Lex::Token, std::string>> Tokens;
        bool DebugMode = false;
        std::ifstream File(argv[1]);

        if(argv[2] != NULL){
            if(std::string(argv[2]) == "-debug"){
                DebugMode = true;
            }
            else if(std::string(argv[2]) == "-compile"){
                throw error::RendorException("Compiling not yet supported");
            }
        }

        {
            std::string AllCode = PreProcess(File);
            Lex::Lexer RenLexer;


            // Tokenizes the AllCode string
            Tokens = RenLexer.Tokenize(AllCode);

            if(DebugMode){
                for(auto const& [token, value] : Tokens){
                    std::cout << static_cast<std::underlying_type<Lex::ID>::type>(token) << " " << value << ";" << std::endl;
                }
            }
        }

        File.close();
        return EXIT_SUCCESS;
    }
    catch(const std::exception& exp){             
        std::cout << exp.what() << std::endl;   
        return EXIT_FAILURE;
    }
    return 0;
}


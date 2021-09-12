/*
---------------------------------
---------------------------------
PLEASE READ BEFORE CONTINUING 
---------------------------------
---------------------------------

This is the main file for Project Rendor(by Mahid Sheikh aka StandingPad Animations(https://twitter.com/StandingPadYT)), 
an updated version of the EnderKnight Programming Language by Mahid Sheikh.

This project is open sourced, as long as you give credit to Mahid Sheikh in the main file of the source code.
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
#include "PythonStatements.hpp"



int main(int argc, char *argv[]){
    /* 
    Sets variables and initializes them.
    std::ifstream checks the file argument(arg[1]) and then takes in the file contents.
    */
    try{
        bool DebugMode = false;
        std::ifstream File(argv[1]);
        // if(argv[2] != NULL){
        //     if(argv[2] == "-debug"){
        //         DebugMode = true;
        //     }
        //     else if(argv[2] == "-compile"){
        //         throw error::RendorException("Compiling not yet supported");
        //     }
        // }


        Lex::Lexer RenLexer;
        py::Python RendorPython;

        /* 
        This part of the program reads the individual lines in the ek file(argv[1]) and checks to see if the size is above 0. 
        If it is, CodeLine.size() is true and false if it is 0.
        Because of this, we can just put if(Line.size()) which will check if it equals true or not, saving some space in the code.
        */
        std::string AllCode;
        for(std::string CodeLine; std::getline(File, CodeLine);){
            if(CodeLine.size()){
                CodeLine = RendorPython.reduce(CodeLine, " ", " \t");
                AllCode = AllCode + CodeLine + '\n';
            }
        }


        // Tokenizes the AllCode string
        std::vector<std::pair<Lex::Token, std::string_view>> Tokens = RenLexer.Tokenize(AllCode);

        if(DebugMode){
            for(auto const& [token, value] : Tokens){
                std::cout << static_cast<int>(token) << " " << value << std::endl;
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

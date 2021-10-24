#include <iostream>
#include <fstream>
#include "RendorInterpreter/Interpreter.hpp"
#include "Exceptions.hpp"

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>

int main(int argc, char *argv[]){
    try{
        boost::filesystem::path AbsPath(argv[1]);
        std::string AbsPathExt = AbsPath.extension().string();
        if(AbsPathExt == ".Cren"){
            std::ifstream File(AbsPath.string());
            if(File.is_open()){
                ExecuteByteCode(File);
            } else {
                throw error::RendorException("File is not open");
            }
        } else{
            throw error::RendorException("rendor only allows .Cren files");
        }
        return EXIT_SUCCESS;
    }
    catch(std::exception& exp){
        std::cout << exp.what() << std::endl;
        return EXIT_FAILURE;
    }
}
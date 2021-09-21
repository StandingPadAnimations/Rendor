#include "Preprocessor.hpp"

void IfFileExists(std::ifstream& Filename){
    if(!Filename){
        throw error::RendorException("File does not exist");
    }
}
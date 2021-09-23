#pragma once
#include <fstream>
#include <string>
#include "Exceptions.hpp"
#include "ExtraFunctions.hpp"

enum class PreprocessName{
    preprocess_main,
    import
};

// main function
std::string PreProcess(std::ifstream&);

// functions to check for stuff
std::string FileValidityCheck(std::ifstream&);
void IfFileExists(std::ifstream&);
std::string LookForEntryPoint(std::ifstream&);
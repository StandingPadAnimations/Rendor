#include "Preprocessor.hpp"

std::string PreProcess(std::ifstream& File){
    // If the file exists and has an entry point
    std::string AllCode = FileValidityCheck(File);;
    return AllCode;
}

std::string FileValidityCheck(std::ifstream& Filename){
    IfFileExists(Filename);
    std::string ReturnCode = LookForEntryPoint(Filename);
    return ReturnCode;
}

void IfFileExists(std::ifstream& Filename){
    if(!Filename){
        throw error::RendorException("File does not exist");
    }
}

std::string LookForEntryPoint(std::ifstream& Filename){
    PreprocessName __RendorName__ = PreprocessName::preprocess_main;
    bool FoundBeginingOfEntryPoint = false;
    bool FoundEndOfEntryPoint = false;
    std::string AllCode;
    ex::Extra RendorExtra;
    
    // go over each line
    for(std::string PreProcessLine; std::getline(Filename, PreProcessLine);){
        // Check if line contains preprocessor function with the # prefix
        if(PreProcessLine[0] == '#'){
            // function name
            std::string PreProcessFunctionName = PreProcessLine.substr(0, PreProcessLine.find_first_of(' '));

            // main function
            if(PreProcessFunctionName == "#rdef"){
                switch(__RendorName__){
                    case PreprocessName::preprocess_main:
                        FoundBeginingOfEntryPoint = true;
                        break;

                    case PreprocessName::import:
                        break;
                }
            }
            //main function end
            else if(PreProcessFunctionName == "#rdef_end"){
                switch(__RendorName__){
                    case PreprocessName::preprocess_main:
                        switch(FoundBeginingOfEntryPoint){
                            case true:
                                FoundEndOfEntryPoint = true;
                                break;
                            
                            case false:
                                error::RendorException("#rdef_end found without clear beginning of entry function");
                        }
                        break;
                        
                    case PreprocessName::import:
                        break;
                }
                
            }
        }
        // add to allcode string
        PreProcessLine = RendorExtra.reduce(PreProcessLine, " ", " \t");
        AllCode = AllCode + PreProcessLine + '\n';
    }

    if((FoundBeginingOfEntryPoint == true) && (FoundEndOfEntryPoint == true)){
        return AllCode;
    }
    throw error::RendorException("Entry Point not defined");
}
#include "Preprocessor.hpp"

std::string PreProcess(std::ifstream& __File__){
    // If the file exists and has an entry point
    std::string __AllCode__ = FileValidityCheck(__File__);;
    return __AllCode__;
}

std::string FileValidityCheck(std::ifstream& __Filename__){
    IfFileExists(__Filename__);
    std::string ReturnCode = LookForEntryPoint(__Filename__);
    return ReturnCode;
}

void IfFileExists(std::ifstream& __Filename__){
    if(!__Filename__){
        throw error::RendorException("File does not exist");
    }
}

std::string LookForEntryPoint(std::ifstream& __Filename__){
    PreprocessName __RendorName__ = PreprocessName::preprocess_main;
    bool __FoundBeginingOfEntryPoint__ = false;
    bool __FoundEndOfEntryPoint__ = false;
    std::string __AllCode__;
    ex::Extra __RendorExtra__;
    
    // go over each line
    for(std::string __PreProcessLine__; std::getline(__Filename__, __PreProcessLine__);){
        // Check if line contains preprocessor function with the # prefix
        if(__PreProcessLine__[0] == '#'){
            // function name
            std::string __PreProcessFunctionName__ = __PreProcessLine__.substr(0, __PreProcessLine__.find_first_of(' '));

            // main function
            if(__PreProcessFunctionName__ == "#rdef"){
                switch(__RendorName__){
                    case PreprocessName::preprocess_main:
                        __FoundBeginingOfEntryPoint__ = true;
                        break;

                    case PreprocessName::import:
                        break;
                }
            }
            //main function end
            else if(__PreProcessFunctionName__ == "#rdef_end"){
                switch(__RendorName__){
                    case PreprocessName::preprocess_main:
                        switch(__FoundBeginingOfEntryPoint__){
                            case true:
                                __FoundEndOfEntryPoint__ = true;
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
        __PreProcessLine__ = __RendorExtra__.reduce(__PreProcessLine__, " ", " \t");
        __AllCode__ = __AllCode__ + __PreProcessLine__ + '\n';
    }

    if((__FoundBeginingOfEntryPoint__ == true) && (__FoundEndOfEntryPoint__ == true)){
        return __AllCode__;
    }
    throw error::RendorException("Entry Point not defined");
}
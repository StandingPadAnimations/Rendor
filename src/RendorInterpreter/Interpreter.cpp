#include "RendorInterpreter/Interpreter.hpp"

void RENDOR_ECHO_FUNCTION(std::string_view EchoValue);

void ExecuteByteCode(std::ifstream& File){
    std::vector<std::unique_ptr<Variable>> Variables;
    std::cout.sync_with_stdio(false); // Makes cout faster by making it not sync with C print statements 

    // ! Make this implementation better
    std::string LoadedConstant = ""; // For constants 
    std::string FastLoadedVariable = ""; // For variables that have been fast loaded
    
    for(std::string ByteCodeOperation; std::getline(File, ByteCodeOperation);){
        size_t ByteCodeSpaceIndex = ByteCodeOperation.find_first_of(" ");
        int ByteCodeSize = ByteCodeOperation.size();
        
        if(ByteCodeSize == 0){
            continue;
        }

        std::string_view Command(ByteCodeOperation.c_str(), ByteCodeSpaceIndex);
        std::string_view Args(ByteCodeOperation.c_str() + ByteCodeSpaceIndex + 1, ByteCodeSize - (ByteCodeSpaceIndex + 1));


        if(Command == "LOAD"){
            switch(Args[0]){
                case '0':
                case '1':
                    break;
            }
        }

        else if(Command == "END"){
            break;
        }

        else if(Command == "ASSIGN"){
            Variables.push_back(std::make_unique<Variable>(std::string{Args}));
            auto& AssignVariable = *Variables.back();
            if(boost::algorithm::contains(LoadedConstant, "_&")){
                std::string_view VariableRefName(LoadedConstant.c_str()+4, LoadedConstant.size()-5);

                if(boost::algorithm::contains(LoadedConstant, "_&&")){
                    throw error::RendorException((boost::format("References are not supported; Error occured with %s") % std::string{VariableRefName}).str());
                }
                
                for(auto const& Variable : Variables){
                    if(Variable->Name == VariableRefName){
                        LoadedConstant = (boost::format("%s%s") % LoadedConstant[0] % Variable->ValueClass->Value).str();
                        break;
                    }
                    else if((Variable == Variables.back()) && (Variable->Name != VariableRefName)){
                        throw error::RendorException("WTH Error; Undefined Variable, \nthis is not supposed to happen at runtime. Please post an issue on the Rendor Github\n");
                    }
                }
            }
            switch(LoadedConstant[0]){
                case '0':
                    LoadedConstant.erase(0, 1);
                    AssignVariable.ValueClass = std::make_unique<Int>(LoadedConstant);
                    break;
                    
                case '1':
                    LoadedConstant.erase(0, 1);
                    AssignVariable.ValueClass = std::make_unique<Float>(LoadedConstant);
                    break;

                case '2':
                    LoadedConstant.erase(0, 1);
                    AssignVariable.ValueClass = std::make_unique<String>(LoadedConstant);
                    break;

                case '3':
                    LoadedConstant.erase(0, 1);
                    AssignVariable.ValueClass = std::make_unique<Bool>(LoadedConstant);
                    break;

                case '4':
                    break;
            }
            LoadedConstant = "";
            continue;
        }

        else if(Command == "CONST"){
            std::string StringArgs{Args};
            std::replace_if(StringArgs.begin(), StringArgs.end(),
            [] 
            (const char& c) 
            {
                if(
                (c == '(') || 
                (c == ')')
                ){
                    return true;
                } else {
                    return false;
                }
            },
            NULL);
            LoadedConstant = StringArgs;
            continue;
        }

        else if(Command == "FAST_LOAD"){
            for(const auto& CurrentVariable : Variables){
                if(CurrentVariable->Name == Args){
                    FastLoadedVariable = CurrentVariable->ValueClass->Value;
                    break;
                }
            }
        }

        else if(Command == "ECHO"){ // For the echo function 
            RENDOR_ECHO_FUNCTION(FastLoadedVariable);
        }

        else{
            throw error::RendorException("Unknown Bytecode Operator; Perhaps try using a newer version of Rendor");
        }
    }
    File.close();
}

void RENDOR_ECHO_FUNCTION(std::string_view EchoValue){
    std::cout << EchoValue << std::endl;
}
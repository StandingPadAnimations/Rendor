#include "RendorInterpreter/Interpreter.hpp"

void RENDOR_ECHO_FUNCTION(std::string &EchoValue);

void ExecuteByteCode(std::ifstream& File){
    std::vector<std::unique_ptr<Variable>> Variables;
    
    for(std::string LineofCode; std::getline(File, LineofCode);){
        std::string Command = LineofCode.substr(0, LineofCode.find_first_of(" "));
        std::string Args = LineofCode.substr(LineofCode.find_first_of(" ")+1, LineofCode.size());

        if(Command == "LOAD"){
            switch(std::stoi(Args)){
                case 0:
                case 1:
                    break;
            }
        }

        else if(Command == "END"){
            continue;
        }

        else if(Command == "ASSIGN"){
            Variables.push_back(std::make_unique<Variable>(Args));
            continue;
        }

        else if(Command == "STRING_CONST"){
            auto& VariablesNode = static_cast<Variable&>(*Variables.back());
            VariablesNode.ValueClass = std::make_unique<String>(Args);
        }

        else if(Command == "ECHO"){
            if(Args.find_first_of("\"'") == std::string::npos){
                for(const auto& Variable : (Variables)){
                    if(Variable->Name == Args){
                        RENDOR_ECHO_FUNCTION(Variable->ValueClass->Value);
                    } else{
                        continue;
                    }
                }
            }   
        }
    }
    File.close();
}

void RENDOR_ECHO_FUNCTION(std::string &EchoValue){
    std::cout.sync_with_stdio(false);
    std::cout << EchoValue << std::endl;
}
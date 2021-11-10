#include "RendorInterpreter/Interpreter.hpp"

void RENDOR_ECHO_FUNCTION (std::string_view EchoValue);

void ExecuteByteCode (std::ifstream& File)
{
    std::map<std::string, std::unique_ptr<Variable>> Variables;
    std::cout.sync_with_stdio(false); // Makes cout faster by making it not sync with C print statements(We're not using C)

    Variables["CONSTANT"] = std::make_unique<Variable>("CONSTANT");
    Variables["CONSTANT"]->ValueClass = std::make_unique<Constant>("NULL", '-');
    
    for (std::string ByteCodeOperation; std::getline(File, ByteCodeOperation);)
    {
        int ByteCodeSize = ByteCodeOperation.size();
        if (ByteCodeSize == 0)
        {
            continue;
        }
        size_t ByteCodeSpaceIndex = ByteCodeOperation.find_first_of(" ");
        std::string_view Command(ByteCodeOperation.c_str(), ByteCodeSpaceIndex);
        std::string_view Args(ByteCodeOperation.c_str() + ByteCodeSpaceIndex + 1, ByteCodeSize - (ByteCodeSpaceIndex + 1));


        if (Command == "CONST") // when a constant is declared 
        { 
            auto& ConstantVariable = *Variables["CONSTANT"];
            auto& ConstantValueClass = dynamic_cast<Constant&>(*ConstantVariable.ValueClass); // get the Constant object from the CONSTANT variable 
            bool AlreadyHandled = false; // to prevent segementation fault errors that occur

            switch (Args[0])
            {
                case '0':
                    ConstantValueClass.ConstVariableType = 'N';
                    break;
                case '1':
                    ConstantValueClass.ConstVariableType = 'F';
                    break;
                case '2':
                    ConstantValueClass.ConstVariableType = 'S';
                    break;
                case '3':
                    ConstantValueClass.ConstVariableType = 'B';
                    break;
                
                case '4':
                    const std::string Result = PostFixEval(Args.substr(2, Args.size()-2), Variables); 
                    if (Result.find_first_of(".") == std::string::npos)
                    {
                        ConstantValueClass.ConstVariableType = 'N';
                    } 
                    else
                    {
                        ConstantValueClass.ConstVariableType = 'F';
                    }
                    ConstantValueClass.Value = Result;
                    AlreadyHandled = true;
                    break;
            }
            if ( // Checks if variable is a copy of another
            (Args[2] == '_') && 
            (Args[3] == '&')) 
            { 
                const auto CopiedVariableName = std::string{Args.substr(4, Args.size()-4)};

                if(AlreadyHandled){ // to see if it was alreday added by the switch statement above
                    continue;
                }
                else if(Variables.count(CopiedVariableName) == 0){ // Checks if variable exists in the first place to avoid segementation fault errors
                    throw error::RendorException((boost::format("Variable %s does not exist!") % CopiedVariableName).str());
                }

                const auto& CopiedVariable = (*Variables[CopiedVariableName]); // if above checks pass, then retrive variable from map
                ConstantValueClass.Value = CopiedVariable.ValueClass->Value;
            } 
            else if(!AlreadyHandled)
            {
                ConstantValueClass.Value = std::string{Args.substr(2, Args.size()-2)};
            }
        }

        else if (Command == "ASSIGN")
        {  
            Variables[std::string{Args}] = std::make_unique<Variable>(std::string{Args});

            auto& VariableBeingAssigned = *Variables[std::string{Args}];
            const auto& ConstantValueClass = dynamic_cast<Constant&>(*Variables["CONSTANT"]->ValueClass); // get the Constant object from the CONSTANT variable 
            
            switch (ConstantValueClass.ConstVariableType)
            {
                case 'N':
                    VariableBeingAssigned.ValueClass = std::make_unique<Int>(ConstantValueClass.Value);
                    break;
                case 'F':
                    VariableBeingAssigned.ValueClass = std::make_unique<Float>(ConstantValueClass.Value);
                    break;
                case 'S':
                    VariableBeingAssigned.ValueClass = std::make_unique<String>(ConstantValueClass.Value);
                    break;
                case 'B':
                    VariableBeingAssigned.ValueClass = std::make_unique<Bool>(ConstantValueClass.Value);
                    break;
                case 'A':
                    throw error::RendorException("WTH Error; Arithmethic should have already been converted to int/float. Please report this as an issue on the Rendor Github.");
            }
        }

        else if (Command == "ECHO")
        {
            const auto& EchoArgs = (*Variables[std::string{Args}]);
            RENDOR_ECHO_FUNCTION(EchoArgs.ValueClass->Value); 
        }

        else if (Command == "INCREMENT")
        {
            auto& IncrementedVariable = dynamic_cast<Int&>(*Variables[std::string{Args}]->ValueClass);
            int ValueOfVariable = IncrementedVariable.RetriveVariable();
            ++ValueOfVariable;
            IncrementedVariable.Value = std::to_string(ValueOfVariable);
        }

        else if (Command == "DECREMENT")
        {
            auto& IncrementedVariable = dynamic_cast<Int&>(*Variables[std::string{Args}]->ValueClass);
            int ValueOfVariable = IncrementedVariable.RetriveVariable();
            --ValueOfVariable;
            IncrementedVariable.Value = std::to_string(ValueOfVariable);
        }
    }
    File.close();
}

void RENDOR_ECHO_FUNCTION (std::string_view EchoValue)
{
    std::cout << EchoValue << std::endl;
}
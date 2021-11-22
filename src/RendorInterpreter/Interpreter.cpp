#include "RendorInterpreter/Interpreter.hpp"

void RENDOR_ECHO_FUNCTION (std::string_view EchoValue);

void engineinterpreter::ExecuteByteCode (std::ifstream& File)
{
    std::cout.sync_with_stdio(false); // Makes cout faster by making it not sync with C print statements(We're not using C)
    std::map<std::string, std::unique_ptr<Variable>> Variables;
    std::map<std::string, std::vector<std::string>> Functions {"echo", {"EchoValue"}};

    // Constant related stuff
    boost::circular_buffer<std::string> Constants(2);
    int ConstantIndex = 0;

    // Call Stack and scope related stuff
    std::vector<std::string> CallStack(690);
    uint32_t Scope = 0;

    for (std::string ByteCodeOperation; std::getline(File, ByteCodeOperation);)
    {
        int ByteCodeSize = ByteCodeOperation.size();
        if (ByteCodeSize == 0)
        {
            continue;
        }
        size_t ByteCodeSpaceIndex = ByteCodeOperation.find_first_of(" ", 0);
        size_t CommandSpaceIndex = ByteCodeOperation.find_first_of(" ", ByteCodeSpaceIndex + 1);

        std::string_view Index(ByteCodeOperation.begin(), ByteCodeOperation.begin() + ByteCodeSpaceIndex);
        std::string_view Command(ByteCodeOperation.begin() + (ByteCodeSpaceIndex + 1), ByteCodeOperation.begin() + CommandSpaceIndex);
        std::string_view Args(ByteCodeOperation.begin() + (CommandSpaceIndex + 1), ByteCodeOperation.end());

        if (Command == "CONST") // Constants 
        {
            Constants.push_back(std::string{Args});
            switch (ConstantIndex)
            {
                case 0:
                {
                    ++ConstantIndex;
                    break;
                }
                
                case 1:
                {
                    --ConstantIndex;
                    break;
                }
            }
        }

        else if (Command == "ASSIGN") // Assigning variables
        {
            std::string Name{Args};
            std::string_view Value{Args.begin() + 2, Args.end()};
            Variables[Name] = std::make_unique<Variable>(Name); // Create variable object

            switch (Constants[ConstantIndex][0])
            {
                case '0':
                {
                    Variables[Name]->ValueClass = std::make_unique<Int>(std::string{Value});
                    break;
                }
                case '1':
                {
                    Variables[Name]->ValueClass = std::make_unique<Float>(std::string{Value});
                    break;
                }
                case '2':
                {
                    Variables[Name]->ValueClass = std::make_unique<String>(std::string{Value});
                    break;
                }
                case '3':
                {
                    Variables[Name]->ValueClass = std::make_unique<Bool>(std::string{Value});
                    break;
                }
            }
        }

        else if (Command == "CALL") // Calling functions
        {
            if (Functions.find(std::string{Args}) == Functions.end())
            {
                throw error::RendorException((boost::format("Function %s does not exist") % Args).str());
            }
            else 
            {
                CallStack.emplace_back(std::string{Args});
                ++Scope;
            }
        }

        else if (Command == "CALL_ARG") // Arguments for functions
        {

        }
    }

    File.close();
}

void RENDOR_ECHO_FUNCTION (std::vector<std::string> EchoArgs)
{
    std::string *EchoValue = &EchoArgs[0];
    std::cout << EchoValue << std::endl;
}
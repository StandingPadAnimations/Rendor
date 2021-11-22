#include "RendorInterpreter/Interpreter.hpp"

void RENDOR_ECHO_FUNCTION (std::string_view EchoValue);

void engineinterpreter::ExecuteByteCode (std::ifstream& File)
{
    std::map<std::string, std::unique_ptr<Variable>> Variables;
    std::cout.sync_with_stdio(false); // Makes cout faster by making it not sync with C print statements(We're not using C)
    
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

        std::cout << Index << " " << Command << " " << Args << " " << std::endl;

    }
    File.close();
}

void RENDOR_ECHO_FUNCTION (std::string_view EchoValue)
{
    std::cout << EchoValue << std::endl;
}
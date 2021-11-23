#include "RendorInterpreter/Interpreter.hpp"

// the actual loop function 
static void ByteCodeLoop(bool DefineMode, std::vector<std::string> ByteCode);

// All built in functions
Variable RENDOR_ECHO_FUNCTION (std::vector<std::string> EchoArgs);

void engineinterpreter::ExecuteByteCode (std::ifstream& File)
{
    std::cout.sync_with_stdio(false); // Makes cout faster by making it not sync with C print statements(We're not using C)

    std::vector<std::string> ByteCode;
    bool DefineMode = true; // Before actual execution of the main function 

    {
        std::string ByteCodeOperation;

        while (std::getline(File, ByteCodeOperation))
        {
            ByteCode.emplace_back(ByteCodeOperation);
        }
    }

    ByteCodeLoop(DefineMode, ByteCode);
    DefineMode = false;
    ByteCodeLoop(DefineMode, ByteCode);

    File.close();
}

static void ByteCodeLoop(bool DefineMode, std::vector<std::string> ByteCode)
{
    static std::map<std::string, std::unique_ptr<Variable>> Variables;

    // Functions
    static std::map<std::string, std::vector<std::string>> FunctionArgs {{"echo", {"EchoValue"}}};
    static std::map<std::string, std::function<Variable(std::vector<std::string>)>> BuiltInFunctions {{"echo", RENDOR_ECHO_FUNCTION}};
    static std::map<std::string, int> UserDefinedFunctions; 

    // Constant related stuff. These aren't static because they don't need to be
    boost::circular_buffer<std::string> Constants(2);
    int ConstantIndex = 0;

    // Call Stack and scope related stuff. Scope is not static as we would rather start in the global scope
    static std::vector<std::string> CallStack(690);
    uint32_t Scope = 0;

    for (auto const& ByteCodeOperation : ByteCode)
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

        if (DefineMode)
        {
            if (Command == "DEFINE") // define functions
            {
                UserDefinedFunctions[std::string{Args}] = std::stoi(std::string{Index});
                ++Scope;
            }

            else if (Command == "FUNCTION") // end scope
            {
                --Scope;
            }

            else if // Global constants
            ((Command == "CONST") &&
            (Scope == 0))
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

            else if // Global variables
            ((Command == "ASSIGN") &&
            (Scope == 0))
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
            
            else // We don't care currently
            {

            }
        }
        else
        {
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
                if (FunctionArgs.find(std::string{Args}) == FunctionArgs.end())
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
    }
}

Variable RENDOR_ECHO_FUNCTION (std::vector<std::string> EchoArgs)
{
    std::string *EchoValue = &EchoArgs[0];
    std::cout << EchoValue << std::endl;
    return Variable("NULL");
}
#include "RendorInterpreter/Interpreter.hpp"

// the actual loop function 
static void ByteCodeLoop(bool DefineMode, std::vector<std::string> ByteCode, size_t& StartIndex);

// All built in functions
std::optional<std::unique_ptr<Type>> RENDOR_ECHO_FUNCTION (std::vector<std::string> EchoArgs);

void engineinterpreter::ExecuteByteCode (std::ifstream& File)
{
    std::cout.sync_with_stdio(false); // Makes cout faster by making it not sync with C print statements(We're not using C)

    std::vector<std::string> ByteCode;
    size_t StartIndex = 0; // Index of the main function 

    {
        std::string ByteCodeOperation;
        while (std::getline(File, ByteCodeOperation))
        {
            ByteCode.emplace_back(ByteCodeOperation);
        }
    }

    ByteCodeLoop(true, ByteCode, StartIndex);
    ByteCodeLoop(false, ByteCode, StartIndex);

    File.close();
}

static void ByteCodeLoop(bool DefineMode, std::vector<std::string> ByteCode, size_t& StartIndex)
{
    // Shortens type names 
    using VariableScopeMap   = std::map<std::string, std::unique_ptr<Variable>>;
    using StringVector       = std::vector<std::string>;
    using RendorFunctionPtr  = std::function<std::optional<std::unique_ptr<Type>>(StringVector)>;

    // Variable related stuff
    static std::vector<VariableScopeMap> VariablesCallStack;

    // Functions
    static std::map<std::string, StringVector> FunctionArgs {{"echo", {"EchoValue"}}};
    static std::map<std::string, RendorFunctionPtr> BuiltInFunctions {{"echo", RENDOR_ECHO_FUNCTION}};
    static std::map<std::string, size_t> UserDefinedFunctions; 

    // Constant related stuff. These aren't static because they don't need to be
    boost::circular_buffer_space_optimized<std::string> Constants(2);
    int ConstantIndex = 0;

    // Call Stack and scope related stuff
    static std::vector<std::pair<std::string, StringVector>> CallStack; // stores name and arguments

    // Non static stuff
    uint32_t Scope = 0;
    VariablesCallStack.emplace_back(VariableScopeMap()); // Create new map
    VariableScopeMap *Variables = &VariablesCallStack.back();

    if (!Variables) 
    {
        throw error::RendorException("Interpreter Error: RendorEngineVM ptr \"Variables\" is NULL");
    }

    for (size_t Op = StartIndex; Op < ByteCode.size(); ++Op)
    {
        std::string *ByteCodeOperation = &ByteCode[Op]; 
        int ByteCodeSize = ByteCodeOperation->size();
        if (ByteCodeSize == 0)
        {
            continue;
        }
        size_t ByteCodeSpaceIndex  = ByteCodeOperation->find_first_of(" ", 0);
        size_t CommandSpaceIndex   = ByteCodeOperation->find_first_of(" ", ByteCodeSpaceIndex + 1);

        std::string_view Index    (ByteCodeOperation->begin(), ByteCodeOperation->begin() + ByteCodeSpaceIndex);
        std::string_view Command  (ByteCodeOperation->begin() + (ByteCodeSpaceIndex + 1), ByteCodeOperation->begin() + CommandSpaceIndex);
        std::string_view Args     (ByteCodeOperation->begin() + (CommandSpaceIndex + 1), ByteCodeOperation->end());

        if (DefineMode)
        {
            if (Command == "DEFINE") // define functions
            {
                UserDefinedFunctions[std::string{Args}] = std::stoi(std::string{Index});
                if (Args == "main")
                {
                    StartIndex = std::stoi(std::string{Index});
                }
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
                if (Constants.size() == 2)
                {
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
            }

            else if // Global variables
            ((Command == "ASSIGN") &&
            (Scope == 0))
            {
                std::string Name{Args};
                std::string VariableValue = Constants[ConstantIndex];
                std::string_view Value{VariableValue.begin() + 2, VariableValue.end()};

                if (Variables->find(Name) == Variables->end())
                {
                    (*Variables)[Name] = std::make_unique<Variable>(Name); // Create variable object
                }
                else {
                    // Ignore variable creation 
                }

                switch (Constants[ConstantIndex][0])
                {
                    case '0':
                    {
                        (*Variables)[Name]->m_ValueClass = std::make_unique<Int>(std::string{Value});
                        break;
                    }
                    case '1':
                    {
                        (*Variables)[Name]->m_ValueClass = std::make_unique<Float>(std::string{Value});
                        break;
                    }
                    case '2':
                    {
                        (*Variables)[Name]->m_ValueClass = std::make_unique<String>(std::string{Value});
                        break;
                    }
                    case '3':
                    {
                        (*Variables)[Name]->m_ValueClass = std::make_unique<Bool>(std::string{Value});
                        break;
                    }
                }
            }
            
            else // We don't care currently
            {
                continue;
            }
        }
        else
        {
            if (Command == "CONST") // Constants 
            {
                Constants.push_back(std::string{Args});

                if (Constants.size() == 2)
                {
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
            }

            else if (Command == "DEFINE") // define functions
            {
                if (UserDefinedFunctions.find(std::string{Args}) == UserDefinedFunctions.end())
                {
                    throw error::RendorException((boost::format("Function %s has not been defined!") % Args).str());
                }
            }

            else if (Command == "ASSIGN") // Assigning variables
            {
                std::string Name{Args};
                std::string VariableValue = Constants[ConstantIndex];
                std::string_view Value{VariableValue.begin() + 2, VariableValue.end()};

                if (Variables->find(Name) == Variables->end())
                {
                    (*Variables)[Name] = std::make_unique<Variable>(Name); // Create variable object
                }
                else {
                    // Ignore variable creation 
                }

                switch (Constants[ConstantIndex][0])
                {
                    case '0':
                    {
                        (*Variables)[Name]->m_ValueClass = std::make_unique<Int>(std::string{Value});
                        break;
                    }
                    case '1':
                    {
                        (*Variables)[Name]->m_ValueClass = std::make_unique<Float>(std::string{Value});
                        break;
                    }
                    case '2':
                    {
                        (*Variables)[Name]->m_ValueClass = std::make_unique<String>(std::string{Value});
                        break;
                    }
                    case '3':
                    {
                        (*Variables)[Name]->m_ValueClass = std::make_unique<Bool>(std::string{Value});
                        break;
                    }
                }
            }

            else if (Command == "CALL") // Calling functions
            {
                if 
                ((BuiltInFunctions.find(std::string{Args}) == BuiltInFunctions.end()) &&
                (UserDefinedFunctions.find(std::string{Args}) == UserDefinedFunctions.end()))
                {
                    throw error::RendorException((boost::format("Call error: Function \"%s\" does not exist") % Args).str());
                }
                else 
                {
                    CallStack.emplace_back(std::string{Args}, std::vector<std::string>());
                    ++Scope;
                }
            }

            else if (Command == "CALL_ARG") // Arguments for functions
            {
                if 
                ((Args[0] == '_') &&
                (Args[1] == '&'))
                {
                    std::string_view Variable(Args.begin() + 2, Args.end());
                    if 
                    ((Variables->find(std::string{Variable}) == Variables->end()) &&
                    (VariablesCallStack[0].find(std::string{Variable}) != VariablesCallStack[0].end()))
                    {
                        CallStack.back().second.emplace_back(VariablesCallStack[0][std::string{Variable}]->m_ValueClass->m_Value); // Add argument
                    }

                    else if (Variables->find(std::string{Variable}) != Variables->end())
                    {
                        CallStack.back().second.emplace_back((*Variables)[std::string{Variable}]->m_ValueClass->m_Value); // Add argument
                    }

                    else 
                    {
                        throw error::RendorException((boost::format("Variable \"%s\" has not been defined") % Variable).str());
                    }
                }
                else
                {
                    CallStack.back().second.emplace_back(std::string{Args}); // Add argument
                }
            }

            else if (Command == "FINALIZE_CALL")
            {
                if (BuiltInFunctions.find(std::string{Args}) != BuiltInFunctions.end())
                {
                    size_t FunctionArgsSize = FunctionArgs[std::string{Args}].size();
                    if (CallStack.back().second.size() > FunctionArgsSize)
                    {
                        throw error::RendorException((boost::format("\"%s\" takes %s argument(s); Too many arguments supplied") % Args % FunctionArgsSize).str());
                    }
                    BuiltInFunctions[std::string{Args}](CallStack.back().second);
                }

                else if (UserDefinedFunctions.find(std::string{Args}) != UserDefinedFunctions.end())
                {
                    ByteCodeLoop(false, ByteCode, UserDefinedFunctions[std::string{Args}]);
                }

                else 
                {
                    throw error::RendorException((boost::format("Finalize Call Error: Function \"%s\" does not exist") % Args).str());
                }
            }

            else if (Command == "FUNCTION") // Ending a function  
            {
                if (Args == "END")
                {
                    VariablesCallStack.pop_back(); // remove local scope variables 
                    return;
                }
            }

            else 
            {
                throw error::RendorException((boost::format("Error: Can not reconize \"%s\"") % (*ByteCodeOperation)).str());
            }
        }
    }
}

std::optional<std::unique_ptr<Type>> RENDOR_ECHO_FUNCTION (std::vector<std::string> EchoArgs)
{
    std::cout << EchoArgs[0] << std::endl;
    return {};
}
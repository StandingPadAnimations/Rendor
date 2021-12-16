#include "RendorInterpreter/Interpreter.hpp"

/* -------------------------------------------------------------------------- */
/*                          Execute ByteCode Function                         */
/* -------------------------------------------------------------------------- */

void Interpreter::ExecuteByteCode(std::ifstream& File)
{
    std::cout.sync_with_stdio(false); // Makes cout faster by making it not sync with C print statements(We're not using C)

    std::vector<std::string> ByteCode;

    {
        std::string ByteCodeOperation;
        while (std::getline(File, ByteCodeOperation))
        {
            ByteCode.emplace_back(ByteCodeOperation);
        }
    }

    /* ------------------------- Define stuff like main ------------------------- */
    ByteCodeLoopDefinition(ByteCode, 0);

    /* ------------------------------ Execute code ------------------------------ */
    ByteCodeLoop(ByteCode, UserDefinedFunctions["main"]);

    File.close();
}


/* ------------------------------ ByteCode Loop ----------------------------- */

void Interpreter::ByteCodeLoop(std::vector<std::string>& ByteCode, size_t StartIndex)
{
    for (size_t Op = StartIndex; Op < ByteCode.size(); ++Op)
    {
        /* -------------------------------------------------------------------------- */
        /*                      Preparing bytecode for execution                      */
        /* -------------------------------------------------------------------------- */
        std::string *ByteCodeOperation = &ByteCode[Op]; 
        int ByteCodeSize = ByteCodeOperation->size();

        if (ByteCodeSize == 0)
        {
            continue;
        }

        size_t ByteCodeSpaceIndex  = ByteCodeOperation->find_first_of(" ");
        std::string_view Command  (ByteCodeOperation->begin(), ByteCodeOperation->begin() + ByteCodeSpaceIndex);
        std::string_view Args     (ByteCodeOperation->begin() + (ByteCodeSpaceIndex + 1), ByteCodeOperation->end());

        /* -------------------------------------------------------------------------- */
        /*                            Execution begins here                           */
        /* -------------------------------------------------------------------------- */
        if (Command == "DEFINE")
        {
            VariablesCallStack.emplace_back(VariableScopeMap()); // add Variable map for current scope
            CurrentScopeVariables = &VariablesCallStack.back();
            GlobalVariables = &VariablesCallStack[0];
            
            if (!CurrentScopeVariables)
            {
                throw error::RendorException("Current Scoped variables can't be accessed!");
            }
        }

        /* ---------------------------- Ending functions ---------------------------- */
        else if (Command == "FUNCTION")
        {
            if (Args == "END")
            {
                VariablesCallStack.pop_back();
                GarbageCollector(true); // Remove constants from memory 
                return; 
            }
        }

        /* -------------------------------- Constants ------------------------------- */
        else if 
        (Command == "CONST")
        {
            Constants.push_back(CreateConstant(Args));
            /* -------------------------- Change ConstantIndex -------------------------- */
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

        /* --------------------------- assigning variables -------------------------- */
        else if 
        (Command == "ASSIGN")
        {
            std::string Var{Args};

            /* ------------------------ Check if variable exists ------------------------ */
            if (GlobalVariables->contains(Var))
            {
                (*GlobalVariables)[Var]->m_ValueClass = Constants[ConstantIndex]; // Just change value 
            }
            else if (CurrentScopeVariables->contains(Var))
            {
                (*CurrentScopeVariables)[Var]->m_ValueClass = Constants[ConstantIndex]; // Just change value 
            }
            else 
            {
                (*CurrentScopeVariables)[Var] = std::make_unique<Variable>(Var); // Create new variable object and then change value 
                (*CurrentScopeVariables)[Var]->m_ValueClass = Constants[ConstantIndex];
            }
        }

        /* ---------------------------- calling functions --------------------------- */
        else if (Command == "CALL")
        {
            if (BuiltInFunctions.contains(std::string{Args}))
            {
                FunctionArgsCallStack.emplace_back(std::vector<TypeObject>());
            }
            else if (UserDefinedFunctions.contains(std::string{Args}))
            {
                FunctionArgsCallStack.emplace_back(std::vector<TypeObject>());
            }
            else 
            {
                throw error::RendorException("Function does not exist!");
            }
        }

        else if (Command == "CALL_ARG")
        {
            FunctionArgsCallStack.back().push_back(CreateConstant(Args));
        }

        else if (Command == "FINALIZE_CALL")
        {
            if (BuiltInFunctions.contains(std::string{Args}))
            {
                std::optional<TypeObject> Result = BuiltInFunctions[std::string{Args}](FunctionArgsCallStack.back());
                if (Result.has_value())
                {
                    PermenentMemory.push_back(Result.value());
                    Constants.push_back(PermenentMemory.back());
                }
            }
            else if (UserDefinedFunctions.contains(std::string{Args}))
            {
                ByteCodeLoop(ByteCode, UserDefinedFunctions[std::string{Args}]);
            }
            else 
            {
                throw error::RendorException("Function does not exist!");
            }
            // Clean up when done 
            FunctionArgsCallStack.pop_back(); // Remove arguments from memory 
        }

        else if (Command == "ARGUMENT")
        {
            std::string Var{Args};
            (*CurrentScopeVariables)[Var] = std::make_unique<Variable>(Var); // Create new variable object and then change value 
            (*CurrentScopeVariables)[Var]->m_ValueClass = FunctionArgsCallStack.back().back();
            FunctionArgsCallStack.back().pop_back();
        }

        else 
        {
            throw error::RendorException("Unreconized bytecode operation error: " + *ByteCodeOperation);
        }
    }
}


/* ----------------------------- Definition Loop ---------------------------- */
void Interpreter::ByteCodeLoopDefinition(std::vector<std::string>& ByteCode, size_t StartIndex)
{
    size_t Scope = 0; // Current scope 
    for (size_t Op = StartIndex; Op < ByteCode.size(); ++Op)
    {
        /* -------------------------------------------------------------------------- */
        /*                           Prepare for definition                           */
        /* -------------------------------------------------------------------------- */
        std::string *ByteCodeOperation = &ByteCode[Op]; 
        int ByteCodeSize = ByteCodeOperation->size();
        if (ByteCodeSize == 0)
        {
            continue;
        }

        size_t ByteCodeSpaceIndex  = ByteCodeOperation->find_first_of(" ");
        std::string_view Command  (ByteCodeOperation->begin(), ByteCodeOperation->begin() + ByteCodeSpaceIndex);
        std::string_view Args     (ByteCodeOperation->begin() + (ByteCodeSpaceIndex + 1), ByteCodeOperation->end());

        /* -------------------------------------------------------------------------- */
        /*                           Definition begins here                           */
        /* -------------------------------------------------------------------------- */

        /* ---------------------------- Load Global Scope --------------------------- */
        if (Command == "LOAD")
        {
            if (Args == "0")
            {
                VariablesCallStack.emplace_back(VariableScopeMap()); // add Variable map for the global scope
                GlobalVariables = &VariablesCallStack.back();

                if (!GlobalVariables)
                {
                    throw error::RendorException("Global Variables can't be accesed!");
                }
            }
        }

        /* --------------------------- Defining functions --------------------------- */
        else if (Command == "DEFINE")
        {
            ++Scope;
            UserDefinedFunctions[std::string{Args}] = Op;
        }

        else if (Command == "FUNCTION")
        {
            if (Args == "END")
            {
                --Scope;
            }
        }

        /* -------------------------------- Constants ------------------------------- */
        else if 
        ((Command == "CONST") &&
        (Scope == 0))
        {
            Constants.push_back(CreateConstant(Args));
            /* -------------------------- Change ConstantIndex -------------------------- */
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

        /* ------------------ Making variables in the global scope ------------------ */
        else if 
        ((Command == "ASSIGN") &&
        (Scope == 0))
        {
            std::string Var{Args};

            /* ------------------------ Check if variable exists ------------------------ */
            if (GlobalVariables->contains(Var))
            {
                (*GlobalVariables)[Var]->m_ValueClass = Constants[ConstantIndex]; // Just change value 
            }
            else 
            {
                (*GlobalVariables)[Var] = std::make_unique<Variable>(Var); // Create new variable object and then change value 
                (*GlobalVariables)[Var]->m_ValueClass = Constants[ConstantIndex];
            }
        }
    }
}



/* -------------------------------------------------------------------------- */
/*                          Memory related functions                          */
/* -------------------------------------------------------------------------- */
/* ---------------------------- Garbage Collector --------------------------- */
void Interpreter::GarbageCollector(bool RemoveConsts)
{
    // if we need to remove variables(exiting any scope except functions since the ByteCode loop handles it for us)
    if (DisposedVariables.size())
    {
        for (std::vector<std::string>::reverse_iterator Var = DisposedVariables.rbegin(); Var != DisposedVariables.rend(); ++Var)
        {
            VariablesCallStack.back().erase(*Var); // Removes variables from variables map
            DisposedVariables.pop_back(); // Remove variable so we don't erase it again later
        }
    }

    // If we need to remove consts. Only done at the end of functions because it can be time consuming 
    if (RemoveConsts)
    {
        for (auto Ptr = PermenentMemory.begin(); Ptr != PermenentMemory.end();)
        {
            // If there's only one reference: the original shared_ptr
            if (Ptr->use_count() == 1)
            {
                // erase element from vector
                Ptr = PermenentMemory.erase(Ptr);
            }
            else 
            {
                ++Ptr;
            }
        }
    }
}

/* ------------------------ Function to add constants ----------------------- */
TypeObject Interpreter::CreateConstant(std::string_view Constant)
{
    /* --------------------------------- Strings -------------------------------- */
    if 
    (Constant[0] == '_')
    {
        if (Constant[1] == 'S')
        {
            std::string StringValue{Constant.begin() + 2, Constant.end()};

            auto FindIterator = std::find_if(
                PermenentMemory.begin(), 
                PermenentMemory.end(),
                [&StringValue] (const std::shared_ptr<Type>& Ptr) 
                {
                    return Ptr->m_Value == StringValue;
                }
            );
            
            // Add new string to permenent memory 
            if (FindIterator == PermenentMemory.end())
            {
                PermenentMemory.push_back(std::make_shared<String>(StringValue));
            }
        }
        
        else if (Constant[1] == '&')
        {
            std::string Variable{Constant.begin() + 2, Constant.end()};

            if (GlobalVariables->contains(Variable))
            {
                PermenentMemory.push_back((*GlobalVariables)[Variable]->m_ValueClass);
            }
            else if (CurrentScopeVariables->contains(Variable))
            {
                PermenentMemory.push_back((*CurrentScopeVariables)[Variable]->m_ValueClass); 
            }
        }

        /* ------------------------------ return result ----------------------------- */
        return PermenentMemory.back();
    }

    /* ---------------------- Check if item exists already ---------------------- */
    auto FindIterator = std::find_if(
        PermenentMemory.begin(), 
        PermenentMemory.end(),
        [&Constant] (const std::shared_ptr<Type>& Ptr) 
        {
            return Ptr->m_Value == Constant;
        }
    );

    /* -------------------------- If item doesn't exist ------------------------- */
    if (FindIterator == PermenentMemory.end())
    {
        /* ----------------------------- Ints and floats ---------------------------- */
        if 
        (Constant.find_first_not_of("1234567890") == std::string::npos) 
        {
            if (Constant.find_first_of(".") != std::string::npos)
            {
                PermenentMemory.push_back(std::make_shared<Float>(std::string{Constant}));
            }
            else 
            {
                PermenentMemory.push_back(std::make_shared<Int>(std::string{Constant}));
            }
        }
        
        /* -------------------------------- Booleans -------------------------------- */
        else if 
        ((Constant == "true") ||
        (Constant == "false"))
        {
            PermenentMemory.push_back(std::make_shared<Bool>(std::string{Constant}));
        }

        /* ------------------------------ return result ----------------------------- */
        return PermenentMemory.back();
    }
    else 
    {
        return (*FindIterator);
    }
}

/* -------------------------------------------------------------------------- */
/*                             Built In Functions                             */
/* -------------------------------------------------------------------------- */

std::optional<TypeObject> Interpreter::RENDOR_ECHO_FUNCTION(std::vector<TypeObject>& EchoArgs)
{
    std::cout << EchoArgs[0]->m_Value << std::endl;
    return {};
}

std::optional<TypeObject> Interpreter::RENDOR_INPUT_FUNCTION(std::vector<TypeObject>& InputArgs)
{
    std::string InputString;
    
    std::cout << InputArgs[0]->m_Value;
    std::getline(std::cin, InputString);

    return std::make_unique<String>(InputString);
}

std::optional<TypeObject> Interpreter::RENDOR_SUM_FUNCTION(std::vector<TypeObject>& SumArgs)
{
    if (SumArgs[0]->TypeOfVariable() != VariableType::Int)
    {
        throw error::RendorException("Sum Function error: Argument must be int");
    }
    auto& NumberToSum = static_cast<Int&>(*SumArgs[0]);
    int64_t SumNumber = NumberToSum.RetriveVariable();

    SumNumber = (SumNumber *  (SumNumber + 1)) / 2;

    return std::make_unique<Int>(std::to_string(SumNumber));
}
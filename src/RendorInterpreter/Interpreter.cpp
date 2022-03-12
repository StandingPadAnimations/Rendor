#include "RendorInterpreter/Interpreter.hpp"

/* -------------------------------------------------------------------------- */
/*                          Execute ByteCode Function                         */
/* -------------------------------------------------------------------------- */

void Interpreter::ExecuteByteCode(const boost::interprocess::mapped_region& File)
{
    std::cout.sync_with_stdio(false); // Makes cout faster by making it not sync with C print statements(We're not using C)

    /* ------------------------- Define stuff like main ------------------------- */
    ByteCodeLoopDefinition(File);

    /* ------------------------------ Execute code ------------------------------ */
    ByteCodeLoop(UserDefinedFunctions["main"]->ByteCode);
}


/* ------------------------------ ByteCode Loop ----------------------------- */

void Interpreter::ByteCodeLoop(std::vector<std::string_view>& ByteCode)
{
    for (size_t Op = 0; Op < ByteCode.size(); ++Op)
    {
        /* -------------------------------------------------------------------------- */
        /*                      Preparing bytecode for execution                      */
        /* -------------------------------------------------------------------------- */
        std::string_view ByteCodeOperation = ByteCode[Op];
        int ByteCodeSize = ByteCodeOperation.size();

        if (ByteCodeSize == 0)
        {
            continue;
        }

        size_t ByteCodeSpaceIndex  = ByteCodeOperation.find_first_of(" ");
        std::string_view Command  (ByteCodeOperation.begin(), ByteCodeOperation.begin() + ByteCodeSpaceIndex);
        std::string_view Args     (ByteCodeOperation.begin() + (ByteCodeSpaceIndex + 1), ByteCodeOperation.end());

        if (!ByteCodeMapping.contains(Command))
        {
            throw error::RendorException("Unreconized bytecode operation error: " + std::string{ByteCodeOperation});
        }

        /* -------------------------------------------------------------------------- */
        /*                            Execution begins here                           */
        /* -------------------------------------------------------------------------- */
        switch (ByteCodeMapping.at(std::string{Command}))
        {
            case ByteCodeEnum::DEFINE:
            {
                VariablesCallStack.emplace_back(VariableScopeMap()); // add Variable map for current scope
                CurrentScopeVariables = &VariablesCallStack.back();
                GlobalVariables = &VariablesCallStack[0];
                
                if (!CurrentScopeVariables)
                {
                    throw error::RendorException("Current Scoped variables can't be accessed!");
                }
                break;
            }

            /* ---------------------------- Ending functions ---------------------------- */
            case ByteCodeEnum::FUNCTION:
            {
                if (Args == "END")
                {
                    VariablesCallStack.pop_back();
                    GarbageCollector(); // Remove constants from memory 
                    CurrentScopeVariables = &VariablesCallStack.back();
                    return; 
                }
                break;
            }

            /* -------------------------------- Constants ------------------------------- */
            case ByteCodeEnum::CONST_OP:
            {
                CreateConstant(Args);
                break;
            }

            /* --------------------------- assigning variables -------------------------- */
            case ByteCodeEnum::ASSIGN:
            {
                std::string Var{Args};
                TypeObject Const = std::move(std::get<1>(RendorStack.top()));
                PopStack();

                /* ------------------------ Check if variable exists ------------------------ */
                if (GlobalVariables->contains(Args))
                {
                    (*GlobalVariables)[Var]->m_ValueClass = Const; // Just change value 
                }
                else if (CurrentScopeVariables->contains(Args))
                {
                    (*CurrentScopeVariables)[Var]->m_ValueClass = Const; // Just change value 
                }
                else 
                {
                    (*CurrentScopeVariables)[Var] = std::make_unique<Variable>(Var); // Create new variable object and then change value 
                    (*CurrentScopeVariables)[Var]->m_ValueClass = Const;
                }
                MarkConstantBlack(Const);
                break;
            }

            case ByteCodeEnum::FINALIZE_CALL:
            {
                /* ----------------------- If it's a built in function ---------------------- */
                if (CppFunctions.contains(Args))
                {
                    CppFunctions[std::string{Args}]();
                }

                /* -------------------------- user defined function ------------------------- */
                else if (UserDefinedFunctions.contains(Args))
                {
                    ByteCodeLoop(UserDefinedFunctions[std::string{Args}]->ByteCode);
                }

                /* -------------------------- non-existant function ------------------------- */
                else 
                {
                    throw error::RendorException("Function does not exist!");
                }

                /* ---------------------- clean up after function call ---------------------- */
                break;
            }

            case ByteCodeEnum::OPERATOR:
            {
                TypeObject Const1;
                TypeObject Const2 = Constants[ConstantIndex].lock();

                /* ----------------------------- First Constant ----------------------------- */
                switch (ConstantIndex)
                {
                    case 0:
                    {
                        Const1 = Constants[1].lock();
                        break;
                    }

                    case 1:
                    {
                        Const1 = Constants[0].lock();
                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
                /* ------------------------------- Evaluation ------------------------------- */
                IfStatementBoolResult.emplace_back(Const1->IfStatementMethod(Const2, OperatorMapping[std::string{Args}]));
                break;
            }

            case ByteCodeEnum::JMP_IF_FALSE:
            {
                switch (IfStatementBoolResult.back())
                {
                    case true:
                    {
                        break;
                    }
                    case false:
                    {
                        Op += boost::lexical_cast<size_t>(Args);
                        break;
                    }
                }
                break;
            }

            case ByteCodeEnum::ENDIF:
            {
                switch (IfStatementBoolResult.back())
                {
                    case true:
                    {
                        break;
                    }
                    case false:
                    {
                        break;
                    }
                }
                IfStatementBoolResult.pop_back();
                break;
            }

            default: 
            {
                break;
            }
        }
    }
}


/* ----------------------------- Definition Loop ---------------------------- */
void Interpreter::ByteCodeLoopDefinition(const boost::interprocess::mapped_region& Code)
{
    std::vector<std::string_view>* Scope = nullptr; // Current scope 
    for (auto const ByteCode : RendorMapping::crange(Code))
    {
        /* -------------------------------------------------------------------------- */
        /*                           Prepare for definition                           */
        /* -------------------------------------------------------------------------- */
        std::string_view ByteCodeOperation{ByteCode.begin(), ByteCode.end()}; 
        int ByteCodeSize = ByteCodeOperation.size();
        if (!ByteCodeSize)
        {
            continue;
        }
        if 
        ((ByteCodeOperation.find_first_not_of(" \r\n") != std::string_view::npos) ||
        (ByteCodeOperation.find_last_not_of(" \r\n") != std::string_view::npos))
        {
            size_t Pos1 = ByteCodeOperation.find_first_not_of(" \r\n");
            size_t Pos2 = ByteCodeOperation.find_last_not_of(" \r\n") + (ByteCodeOperation.size() - ByteCodeOperation.find_last_of("\r"));
            ByteCodeOperation = ByteCodeOperation.substr(Pos1, Pos2);
            while 
            ((ByteCodeOperation.back() == '\r') || 
            (ByteCodeOperation.back() == ' '))
            {
                --Pos2;
                ByteCodeOperation = std::string_view{ByteCode.begin(), ByteCode.end()};
                ByteCodeOperation = ByteCodeOperation.substr(Pos1, Pos2);
            }
        }
        else 
        {
            continue;
        }
        
        size_t ByteCodeSpaceIndex  = ByteCodeOperation.find_first_of(" ");
        std::string_view Command  (ByteCodeOperation.begin(), ByteCodeOperation.begin() + ByteCodeSpaceIndex);
        std::string_view Args     (ByteCodeOperation.begin() + (ByteCodeSpaceIndex + 1), ByteCodeOperation.end());

        /* -------------------------------------------------------------------------- */
        /*                           Definition begins here                           */
        /* -------------------------------------------------------------------------- */

        /* ---------------------------- Load Global Scope --------------------------- */
        switch (Interpreter::ByteCodeMapping[std::string{Command}])
        {
            case ByteCodeEnum::LOAD:
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
                break;
            }

            /* --------------------------- Defining functions --------------------------- */
            case ByteCodeEnum::DEFINE:
            {
                UserDefinedFunctions[std::string{Args}] = std::make_unique<Function>();
                Scope = &UserDefinedFunctions[std::string{Args}]->ByteCode;
                break;
            }

            case ByteCodeEnum::FUNCTION:
            {
                Scope->emplace_back(ByteCodeOperation);
                if (Args == "END")
                {
                    Scope = nullptr;
                }
                break;
            }

            /* -------------------------------- Constants ------------------------------- */
            case ByteCodeEnum::CONST_OP:
            {
                if (Scope == 0)
                {
                    break;
                }
                AddToConstantsArray(ParseConstant(Args));
                break;
            }

            /* ------------------ Making variables in the global scope ------------------ */
            case ByteCodeEnum::ASSIGN:
            {
                if (Scope == 0)
                {
                    break;
                }
                std::string Var{Args};
                TypeObject Const = Constants[ConstantIndex].lock();
                MarkConstantBlack(Const);
                /* ------------------------ Check if variable exists ------------------------ */
                if (GlobalVariables->contains(Args))
                {
                    (*GlobalVariables)[Var]->m_ValueClass = Const; // Just change value 
                }
                else 
                {
                    (*GlobalVariables)[Var] = std::make_unique<Variable>(Var); // Create new variable object and then change value 
                    (*GlobalVariables)[Var]->m_ValueClass = Const;
                }
                break;
            }

            default:
            {
                break;
            }
        }
        if (Scope != nullptr)
        {
            Scope->emplace_back(ByteCodeOperation);
        }
    }
}
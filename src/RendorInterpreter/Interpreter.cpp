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
        RendorState RendorStateID = RendorStateIDList.back();
        int ByteCodeSize = ByteCodeOperation.size();

        if (ByteCodeSize == 0)
        {
            continue;
        }

        size_t ByteCodeSpaceIndex  = ByteCodeOperation.find_first_of(" ");
        std::string_view Command  (ByteCodeOperation.begin(), ByteCodeOperation.begin() + ByteCodeSpaceIndex);
        std::string_view Args     (ByteCodeOperation.begin() + (ByteCodeSpaceIndex + 1), ByteCodeOperation.end());

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
                GarbageCollector(); // Remove constants from memory 
                CurrentScopeVariables = &VariablesCallStack.back();
                return; 
            }
        }

        /* -------------------------------- Constants ------------------------------- */
        else if 
        (Command == "CONST")
        {
            switch (RendorStateID)
            {
                /* ----------------------------- Function Calls ----------------------------- */
                case RendorState::FunctionCall:
                {
                    auto [Const1, Const2] = ParseConstant(Args);
                    FunctionArgsCallStack.back().push_back(Const1);

                    if (!Const2.expired())
                    {
                        FunctionArgsCallStack.back().push_back(Const2);
                    }
                    break;
                }

                /* ------------------------------ Default stuff ----------------------------- */
                default:
                {
                    AddToConstantsArray(ParseConstant(Args));
                    break;
                }
            }
        }

        /* --------------------------- assigning variables -------------------------- */
        else if 
        (Command == "ASSIGN")
        {
            std::string Var{Args};
            TypeObject Const = Constants[ConstantIndex].lock();
            MarkConstantBlack(Const);

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
        }

        /* ---------------------------- calling functions --------------------------- */
        else if (Command == "CALL")
        {
            if ((BuiltInFunctions.contains(Args)) || (UserDefinedFunctions.contains(Args)))
            {
                RendorStateIDList.emplace_back(RendorState::FunctionCall);
                FunctionArgsCallStack.emplace_back(TypePtrVector());
            }
            else 
            {
                throw error::RendorException("Function does not exist!");
            }
        }

        else if (Command == "FINALIZE_CALL")
        {
            /* ----------------------- If it's a built in function ---------------------- */
            if (BuiltInFunctions.contains(Args))
            {
                std::optional<TypeObjectPtr> Result = BuiltInFunctions[std::string{Args}](FunctionArgsCallStack.back());
                RendorStateIDList.pop_back();

                if (Result.has_value())
                {
                    switch (RendorStateIDList.back())
                    {
                        case RendorState::FunctionCall:
                        {
                            FunctionArgsCallStack.pop_back();
                            FunctionArgsCallStack.back().push_back(Result.value());
                            break;
                        }
                        default:
                        {
                            FunctionArgsCallStack.pop_back();
                            AddToConstantsArray(Result.value());
                            break;
                        }
                    }
                }
                else 
                {
                    FunctionArgsCallStack.pop_back();
                }
            }

            /* -------------------------- user defined function ------------------------- */
            else if (UserDefinedFunctions.contains(Args))
            {
                ByteCodeLoop(UserDefinedFunctions[std::string{Args}]->ByteCode);
                RendorStateIDList.pop_back();
                FunctionArgsCallStack.pop_back();
            }

            /* -------------------------- non-existant function ------------------------- */
            else 
            {
                throw error::RendorException("Function does not exist!");
            }

            /* ---------------------- clean up after function call ---------------------- */
        }

        else if (Command == "ARGUMENT")
        {
            std::string Var{Args};
            TypeObject Const = FunctionArgsCallStack.back().back().lock();
            MarkConstantBlack(Const);

            /* ----------------------------- Assign Variable ---------------------------- */
            (*CurrentScopeVariables)[Var] = std::make_unique<Variable>(Var); // Create new variable object and then change value 
            (*CurrentScopeVariables)[Var]->m_ValueClass = Const;
            FunctionArgsCallStack.back().pop_back();
        }

        else if (Command == "OPERATOR")
        {
            TypeObject Const1;
            TypeObject Const2 = Constants[ConstantIndex].lock();
            Operator CompOp = Operator::EQUAL;

            /* ----------------------------- Second Constant ---------------------------- */
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
            
            /* -------------------------------- Operator -------------------------------- */
            if (Args == "EQUAL")
            {
                CompOp = Operator::EQUAL;
            }
            else if (Args == "NOT_EQUAL")
            {
                CompOp = Operator::NOT_EQUAL;
            }
            else if (Args == "GREATER_THAN")
            {
                CompOp = Operator::GREATER_THAN;
            }
            else if (Args == "LESS_THAN")
            {
                CompOp = Operator::LESS_THAN;
            }
            else if (Args == "GREATER_OR_EQUAL")
            {
                CompOp = Operator::GREATER_OR_EQUAL;
            }
            else if (Args == "LESS_OR_EQUAL")
            {
                CompOp = Operator::LESS_OR_EQUAL;
            }

            /* ------------------------------- Evaluation ------------------------------- */
            IfStatementBoolResult.emplace_back(Const1->IfStatementMethod(Const2, CompOp));
        }

        else if (Command == "JMP_IF_FALSE")
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
        }

        else if ("ENDIF")
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
        }

        else 
        {
            throw error::RendorException("Unreconized bytecode operation error: " + std::string{ByteCodeOperation});
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
        size_t ByteCodeSpaceIndex  = ByteCodeOperation.find_first_of(" ");
        std::string_view Command  (ByteCodeOperation.begin(), ByteCodeOperation.begin() + ByteCodeSpaceIndex);
        std::string_view Args     (ByteCodeOperation.begin() + (ByteCodeSpaceIndex + 1), ByteCodeOperation.end());

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
            UserDefinedFunctions[std::string{Args}] = std::make_unique<Function>();
            Scope = &UserDefinedFunctions[std::string{Args}]->ByteCode;
        }

        else if (Command == "FUNCTION")
        {
            Scope->emplace_back(ByteCodeOperation);
            if (Args == "END")
            {
                Scope = nullptr;
            }
        }

        /* -------------------------------- Constants ------------------------------- */
        else if 
        ((Command == "CONST") &&
        (Scope == 0))
        {
            AddToConstantsArray(ParseConstant(Args));
        }

        /* ------------------ Making variables in the global scope ------------------ */
        else if 
        ((Command == "ASSIGN") &&
        (Scope == 0))
        {
            
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
        }

        if (Scope != nullptr)
        {
            Scope->emplace_back(ByteCodeOperation);
        }
    }
}

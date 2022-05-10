#include "RendorCompiler/ASTInspection/ASTInspector.hpp"
#include "RendorCompiler/Nodes/Nodes.hpp"
#include <fmt/color.h>
#include <string_view>

void ASTInspector::InspectAST(const NodeObject& Node)
{
    if (Variables.empty())
    {
        Variables.emplace_back(VariableMap());
    }
    switch (Node->Type)
    {
        /* -------------------------------------------------------------------------- */
        /*                                  Variables                                 */
        /* -------------------------------------------------------------------------- */
        case NodeType::AssignVariable:
        {
            auto& AssignmentNode = static_cast<AssignVariable&>(*Node);
            /* ------------------------------- Valid name ------------------------------- */
            if 
            ((AssignmentNode.VariableName.size()) &&
            (!InvalidIdentifier(AssignmentNode.VariableName[0])))
            {
                if (AssignmentNode.Value)
                {
                    /* ----------------------- Inspect value for validity ----------------------- */
                    if (
                    (AssignmentNode.Value->Type == NodeType::Int64)         ||
                    (AssignmentNode.Value->Type == NodeType::Double)        ||
                    (AssignmentNode.Value->Type == NodeType::String)        ||
                    (AssignmentNode.Value->Type == NodeType::Bool)          ||
                    (AssignmentNode.Value->Type == NodeType::Arithmethic)   ||
                    (AssignmentNode.Value->Type == NodeType::FunctionCall))
                    {
                        InspectAST(AssignmentNode.Value);
                        (*CurrentVariables)[AssignmentNode.VariableName] = AssignmentNode.Value->Type;
                    }
                    else 
                    {
                        throw error::CompilerRendorException(
                            fmt::format("Variable has an invalid value; Line {}", Node->LineNumber),
                            Node->LineNumber);
                    }
                }
                else if (AssignmentNode.IsArg)
                {
                    (*CurrentVariables)[AssignmentNode.VariableName] = AssignmentNode.Value->Type;
                }
                else 
                {
                    throw error::CompilerRendorException(
                        fmt::format("Variable has no value; Line {}", Node->LineNumber),
                        Node->LineNumber);
                }
            }
            else 
            {
                throw error::CompilerRendorException(
                    fmt::format("Invalid variable name; Line {}", Node->LineNumber),
                    Node->LineNumber);
            }
            break;
        }

        /* -------------------------------------------------------------------------- */
        /*                                  Function                                  */
        /* -------------------------------------------------------------------------- */
        case NodeType::FunctionCall:
        {
            auto& FunctionCallNode = static_cast<FunctionCall&>(*Node);
            if (FunctionCallNode.Function.empty())
            {
                throw error::CompilerRendorException(
                    fmt::format("Invalid function name; Line {}", Node->LineNumber),
                    Node->LineNumber);
            }

            if (FunctionCallNode.Function == "main")
            {
                throw error::CompilerRendorException(
                    fmt::format("Who the frick calls main?; Line {}", Node->LineNumber),
                    Node->LineNumber);
            }

            // Sort the declarations vector
            std::sort(Declarations.begin(), Declarations.end(), 
            [](Declaration& Dec1, Declaration& Dec2)
            {
                if (Dec1.OriginalName == Dec2.OriginalName) // if both names are the same
                {
                    return Dec1.ArgTypes.size() < Dec2.ArgTypes.size();
                }
                return Dec1.OriginalName < Dec2.OriginalName; // otherwise sort by the alphabet
            });
            
            // Start searching if the function has been defined
            if (RendorEngineCompiler::EngineContext.FunctionTable.contains(FunctionCallNode.Function))
            {
                bool OriginalNameFound = false;
                
                // go over the declarations
                for (auto& FuncDeclaration : Declarations)
                {
                    // if the size or the name aren't equal
                    if (FuncDeclaration.ArgTypes.size() < FunctionCallNode.Args.Types.size())
                    {
                        continue;
                    }

                    // if the original name is not what we're looking for, and we've already found the original name
                    else if 
                    ((FuncDeclaration.OriginalName != FunctionCallNode.Function) &&
                    (OriginalNameFound))
                    {
                        // throw an exception if we reach here
                        throw error::CompilerRendorException(
                            fmt::format("Cannot find compatible declaration to function {}; Line {}", 
                            FunctionCallNode.Function, 
                            Node->LineNumber),
                            Node->LineNumber);
                    }
                    else
                    {
                        OriginalNameFound = true; // Found the original name, so we mark it as true

                        // if the name and vector size are the same, check if the types are equivelent
                        if (FuncDeclaration.IsFunc(FunctionCallNode.ReturnValue, FunctionCallNode.Args.Types))
                        {
                            FunctionCallNode.Function = FuncDeclaration.MangledName;
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
                
            }
            else
            {
                // To visit later
                FunctionCalls.emplace_back(&FunctionCallNode);
                error::LogWarning(fmt::format("Detected call to {} before declaration; Line {}", 
                    FunctionCallNode.Function, 
                    Node->LineNumber));
            }
            
            for (auto const& Arg : FunctionCallNode.Args.ConnectedNodes)
            {
                InspectTypesReferences(Arg->Type, Arg);
            }
            break;
        }

        case NodeType::Edef:
        {
            auto& EdefNode = static_cast<Edef&>(*Node);
            if (EdefNode.Name.empty())
            {
                throw error::CompilerRendorException(fmt::format("Invalid function name; Line {}", EdefNode.LineNumber),
                    EdefNode.LineNumber);
            }

            if (!ReverseTypeTable.contains(EdefNode.ReturnType))
            {
                throw error::CompilerRendorException(fmt::format("Invalid type for function; Line {}", EdefNode.LineNumber),
                    EdefNode.LineNumber);
            }

            if (EdefNode.ReturnType == NodeType::Any)
            {
                error::LogWarning(fmt::format("{} has a dynamic return type. This may cause issues with overloading", EdefNode.Name));
            }

            // Add new variable scope
            AddVariableScope();

            for (const auto& [Var, Type] : EdefNode.Args)
            {
                (*CurrentVariables)[Var] = Type;
            }

            // Inspect the body
            for (const auto& NodeInNode : EdefNode.FunctionBody.ConnectedNodes) // actual body 
            {
                InspectAST(NodeInNode);
            }
            // Destroy variable scope
            DestroyVariableScope();
            
            // Mangle name
            std::string EdefName_Mangled = MangleName(EdefNode.Args, EdefNode.Name, EdefNode.ReturnType);

            // Add function
            RendorEngineCompiler::EngineContext.FunctionTable.emplace(EdefNode.Name);
            for (auto const& CalledFunction : FunctionCalls)
            {
                if (CalledFunction->Function == EdefNode.Name)
                {
                    size_t FunctionCallSize = CalledFunction->Args.ConnectedNodes.size();
                    size_t FunctionArgsSize = EdefNode.Args.size();

                    if (FunctionCallSize < FunctionArgsSize)
                    {
                        throw error::CompilerRendorException(fmt::format("Missing Argument in function call for {}; Line {}", 
                            CalledFunction->Function, 
                            Node->LineNumber),
                            Node->LineNumber);
                    }
                    else if (FunctionCallSize > FunctionArgsSize)
                    {
                        throw error::CompilerRendorException(fmt::format("Too many arguments in function call for {}; Line {}", 
                            CalledFunction->Function, 
                            Node->LineNumber),
                            Node->LineNumber);
                    }

                    // Change name to mangled variant if valid
                    CalledFunction->Function = EdefName_Mangled;
                }
            }
            break;
        }

        case NodeType::FowardEdef:
        {
            auto& ForwardNode = static_cast<FowardEdef&>(*Node);

            if (ForwardNode.ReturnType == NodeType::Any)
            {
                error::LogWarning(fmt::format("{} has a dynamic return type. This may cause issues with overloading", ForwardNode.Name));
            }

            // Create declaration
            Declaration ForwardDeclaration(ForwardNode.Args);
            ForwardDeclaration.OriginalName = ForwardNode.Name;
            ForwardDeclaration.ReturnType   = ForwardNode.Type;
            ForwardDeclaration.MangledName  = MangleName(ForwardNode.Args, ForwardNode.Name, ForwardNode.ReturnType);

            // Add it to vector
            Declarations.push_back(std::move(ForwardDeclaration));

            // Add function to existing functions
            RendorEngineCompiler::EngineContext.FunctionTable.emplace(ForwardNode.Name);
            break;
        }

        case NodeType::IfElse:
        {
            auto& IfNode = static_cast<IfElse&>(*Node);
            if (IfNode.Conditions->Condition1)
            {
                if (IfNode.Conditions->Operator)
                {
                    // If there is an operator, then there has to be an if statement
                    if (IfNode.Conditions->Condition2)
                    {
                        // Check if condition 2 is valid if provided by the user
                        InspectTypesReferences(IfNode.Conditions->Condition2->Type, IfNode.Conditions->Condition2);
                    }
                    else
                    {
                        throw error::CompilerRendorException(
                            fmt::format("If statement contains operator, expected an expression; Line {}", 
                            Node->LineNumber),
                            Node->LineNumber);
                    }
                }
                else 
                {
                    // Make it check if true
                    IfNode.Conditions->Operator = std::make_unique<BiOp>("==", Node->LineNumber);
                    IfNode.Conditions->Condition2 = std::make_unique<Bool>("true", Node->LineNumber);
                }
                
                // Check the validity of condition 1 as that is always provided by the user
                InspectTypesReferences(IfNode.Conditions->Condition1->Type, IfNode.Conditions->Condition1);
            }
            for (const auto& NodeInNode : IfNode.IfElseBody.ConnectedNodes) // actual body 
            {
                InspectAST(NodeInNode);
            }
            break;
        }

        case NodeType::Body:
        {
            // Check if there's a namespace and add the namespace if there is
            auto& BodyNode = static_cast<Body&>(*Node);
            if (!BodyNode.NameSpace.empty())
            {
                NameSpaces.emplace_back(BodyNode.NameSpace);
            }

            for (const auto& NodeInNode : BodyNode.ConnectedNodes) // actual body 
            {
                InspectAST(NodeInNode);
            }
            break;
        }

        case NodeType::Export:
        {
            auto& ExportNode = static_cast<Export&>(*Node);
            for (const auto& NodeInNode : ExportNode.ExportBody.ConnectedNodes) // actual body 
            {
                InspectAST(NodeInNode);
            }
            break;
        }

        case NodeType::Import:
        {
            auto& ImportNode = static_cast<Import&>(*Node);
            if (ImportNode.Name.empty())
            {
                throw error::CompilerRendorException(
                    fmt::format("Missing Import; Line {}", 
                    ImportNode.LineNumber), 
                    ImportNode.LineNumber);
            }

            if (ImportNode.CImport)
            {
                if (!CImports.contains(ImportNode.Name))
                {
                    std::string Path;
                    if (RendorSTDCPP.contains(ImportNode.Name))
                    {
                        Path = fmt::format("{}/{}.{}",CurrentPath, ImportNode.Name, C_Extension_Suffix);
                    }
                    else 
                    {
                        Path = ImportNode.Name + C_Extension_Suffix;
                    }
                    std::string FuncName = "Rendor_Module_" + ImportNode.Name;
                    boost::dll::shared_library CImport(Path);
                    if (!CImport.has(FuncName))
                    {
                        throw error::RendorException("Missing " + FuncName);
                    }
                    std::function<void(RendorState*)> InitFunc = CImport.get<void(RendorState*)>(FuncName);
                    InitFunc(&RendorEngineCompilerState);
                    CImports.emplace(ImportNode.Name);
                }
            }
            break;
        }

        // Nodes that aren't checked will break here to prevent issues
        default:
        {
            break;
        }
    }
}
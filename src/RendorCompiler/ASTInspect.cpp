#include "RendorCompiler/Parser.hpp"

void Parser::DeltaInspectAST(const NodeObject& Node)
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
                    (AssignmentNode.Value->Type == NodeType::Int)           ||
                    (AssignmentNode.Value->Type == NodeType::Double)        ||
                    (AssignmentNode.Value->Type == NodeType::String)        ||
                    (AssignmentNode.Value->Type == NodeType::Bool)          ||
                    (AssignmentNode.Value->Type == NodeType::Arithmethic)   ||
                    (AssignmentNode.Value->Type == NodeType::FunctionCall))
                    {
                        DeltaInspectAST(AssignmentNode.Value);
                        (*CurrentVariables)[AssignmentNode.VariableName] = AssignmentNode.Value->Type;
                    }
                    else 
                    {
                        throw error::RendorException((boost::format("Variable has a invalid value; Line %s") % Node->LineNumber).str());
                    }
                }
                else 
                {
                    throw error::RendorException((boost::format("Variable has no value; Line %s") % Node->LineNumber).str());
                }
            }
            else 
            {
                throw error::RendorException((boost::format("Invalid variable name; Line %s") % Node->LineNumber).str());
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
                throw error::RendorException((boost::format("Invalid function name; Line %s") % Node->LineNumber).str());
            }

            if (Functions.contains(FunctionCallNode.Function))
            {
                if (Functions[FunctionCallNode.Function].size() != FunctionCallNode.Args.size())
                {
                    throw error::RendorException((boost::format("Missing Argument in function call for %s; Line %s") % FunctionCallNode.Function % Node->LineNumber).str());
                }
            }

            for (auto const& Arg : FunctionCallNode.Args)
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
                throw error::RendorException((boost::format("Invalid function name; Line %s") % Node->LineNumber).str());
            }

            // Add new variable scope
            AddVariableScope();
            // Add variables
            for (const auto& [Var, Type] : EdefNode.Args)
            {
                (*CurrentVariables)[Var] = Type;
            }
            // Inspect the body
            for (const auto& Node : EdefNode.FunctionBody.ConnectedNodes) // actual body 
            {
                DeltaInspectAST(Node);
            }
            // Destroy variable scope
            DestroyVariableScope();

            // Add function
            Functions[EdefNode.Name] = FunctionArgsVector();
            Functions[EdefNode.Name].reserve(EdefNode.Args.size());
            std::fill(Functions[EdefNode.Name].begin(), Functions[EdefNode.Name].end(), NodeType::Any);
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
                        break;
                    }
                    else
                    {
                        throw error::RendorException((boost::format("If statement contains operator, expected an expression; Line %s") % Node->LineNumber).str());
                    }
                }
                else 
                {
                    // Make it check if true
                    IfNode.Conditions->Operator = std::make_unique<BiOp>("==", IfNode.LineNumber);
                    IfNode.Conditions->Condition2 = std::make_unique<Bool>("true", IfNode.LineNumber);
                }
                
                // Check the validity of condition 1 as that is always provided by the user
                InspectTypesReferences(IfNode.Conditions->Condition1->Type, IfNode.Conditions->Condition1);
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
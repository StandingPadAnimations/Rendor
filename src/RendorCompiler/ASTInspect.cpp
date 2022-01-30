#include "RendorCompiler/Parser.hpp"

static bool InvalidIdentifier(char& CharactherToCheck);

void Parser::DeltaInspectAST(const NodeObject& Node)
{
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

            Variables[AssignmentNode.VariableName] = AssignmentNode.Value->Type;
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
            break;
        }

        case NodeType::Edef:
        {
            auto& EdefNode = static_cast<Edef&>(*Node);
            if (EdefNode.Name.empty())
            {
                throw error::RendorException((boost::format("Invalid function name; Line %s") % Node->LineNumber).str());
            }

            for (const auto& Node : EdefNode.FunctionBody.ConnectedNodes) // actual body 
            {
                DeltaInspectAST(Node);
            }
            break;
        }

        case NodeType::IfElse:
        {
            auto& IfNode = static_cast<IfElse&>(*Node);
            if (IfNode.Conditions->Condition1)
            {
                if (IfNode.Conditions->Operator)
                {
                    if (IfNode.Conditions->Condition2)
                    {
                        break;
                    }
                    else
                    {
                        throw error::RendorException((boost::format("Lack of second condition in if statement; Line %s") % Node->LineNumber).str());
                    }
                }
                else 
                {
                    // Make it check if true
                    IfNode.Conditions->Operator = std::make_unique<BiOp>("==", IfNode.LineNumber);
                    IfNode.Conditions->Condition2 = std::make_unique<Bool>("true", IfNode.LineNumber);
                }
            }
            break;
        }

        /* -------------------------------------------------------------------------- */
        /*                              Unsupported Nodes                             */
        /* -------------------------------------------------------------------------- */
        default:
        {
            break;
        }
    }
}


static bool InvalidIdentifier(char& CharactherToCheck)
{
    switch (CharactherToCheck)
    {  
        case '0':
        FALLTHROUGH;
        case '1':
        FALLTHROUGH;
        case '2':
        FALLTHROUGH;
        case '3':
        FALLTHROUGH;
        case '4':
        FALLTHROUGH;
        case '5':
        FALLTHROUGH;
        case '6':
        FALLTHROUGH;
        case '7':
        FALLTHROUGH;
        case '8':
        FALLTHROUGH;
        case '9':
        FALLTHROUGH;
        case '&':
        {
            return true;
        }

        default:
        {
            return false;
        }
    }
}
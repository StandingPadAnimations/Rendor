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
                    (AssignmentNode.Value->Type == NodeType::Int)    ||
                    (AssignmentNode.Value->Type == NodeType::Double) ||
                    (AssignmentNode.Value->Type == NodeType::String) ||
                    (AssignmentNode.Value->Type == NodeType::Bool)   ||
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
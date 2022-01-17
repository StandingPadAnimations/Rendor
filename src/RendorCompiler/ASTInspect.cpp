#include "RendorCompiler/Parser.hpp"

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
                    DeltaInspectAST(AssignmentNode.Value);
                }
                else 
                {
                    throw error::RendorException((boost::format("Variable has no value; Line %s")).str());
                }
            }
            else 
            {
                throw error::RendorException((boost::format("Invalid variable name; Line %s")).str());
            }
            break;
        }

        /* -------------------------------------------------------------------------- */
        /*                                  Function                                  */
        /* -------------------------------------------------------------------------- */
        case NodeType::FunctionCall:
        {
            auto& FunctionCallNode = static_cast<FunctionCall&>(*Node);
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
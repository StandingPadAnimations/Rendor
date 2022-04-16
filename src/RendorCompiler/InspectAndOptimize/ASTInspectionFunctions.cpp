#include "RendorCompiler/Parser/Parser.hpp"

void Parser::InspectTypesReferences(const NodeType& Type, const NodeObject& Node)
{
    switch (Type)
    {
        case NodeType::Reference:
        {
            auto& ReferenceNode = static_cast<Reference&>(*Node);
            if (!CurrentVariables->contains(ReferenceNode.Value))
            {
                throw error::RendorException((boost::format("Undefined variable %s; Line %s") % ReferenceNode.Value % Node->LineNumber).str());
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void Parser::AddParentNode(Body* ParentNode)
{
    ParentNodes.emplace_back(ParentNode);
}

bool Parser::InvalidIdentifier(const char& CharactherToCheck)
{
    switch (CharactherToCheck)
    {  
        case '0': // NOLINT
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
        FALLTHROUGH;
        case '@':
        FALLTHROUGH;
        case '$':
        FALLTHROUGH;
        case '#':
        FALLTHROUGH;
        case '\\':
        {
            return true;
        }

        default:
        {
            return false;
        }
    }
}

void Parser::AddVariableScope()
{
    Variables.emplace_back(VariableMap());
    CurrentVariables = &Variables.back();
}

void Parser::DestroyVariableScope()
{
    Variables.pop_back();
    CurrentVariables = &Variables.back();
}
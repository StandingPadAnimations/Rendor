#include "RendorCompiler/ASTInspection/RendorFunctionDeclaration.hpp"
#include <algorithm>
#include <string_view>
#include <ranges>
#include <iomanip>

void FunctionDeclaration::AddFunction(const NodeType ReturnType, std::vector<NodeType>& Types)
{
    DeclarationStorage* TypeStorage = &FunctionDeclarations[TypeIndexTable.at(ReturnType)];
    TypeStorage->Declarations.emplace_back(&Types);
}

std::vector<NodeType>* FunctionDeclaration::GetFunction(const NodeType ReturnType, const std::vector<NodeType>& Types)
{
    DeclarationStorage* TypeStorage = &FunctionDeclarations[TypeIndexTable.at(ReturnType)];

    // sort the vector based on size
    std::sort(TypeStorage->Declarations.begin(), TypeStorage->Declarations.end(), 
    [](const std::vector<NodeType>* v1, const std::vector<NodeType>* v2)
    {
        return v1->size() > v2->size();
    });
    
    auto Iterator = std::find_if(
    TypeStorage->Declarations.begin(), 
    TypeStorage->Declarations.end(), 
    [&Types](const std::vector<NodeType>* Declaration)
    {
        if (Types.size() != Declaration->size())
        {
            return false;
        }
        else if (Types == *Declaration)
        {
            return true;
        }
        else
        {
            if (Declaration->back() != NodeType::Any) // All any types must be at the end, therefore we shouldn't iterate over this declaration
            {
                return false;
            }
            for (size_t index = 0; index < Types.size(); ++index)
            {
                if ((*Declaration)[index] == NodeType::Any)
                {
                    return true; // At this point we can confirm this declaration should be valid
                }
                else if (Types[index] == (*Declaration)[index])
                {
                    continue; // We don't know if it's valid yet
                }
                else
                {
                    break; // It's useless, we need the next declaration
                }
            }
        }
        return false;
    });

    if (Iterator != TypeStorage->Declarations.end())
    {
        return *Iterator;
    }
    return nullptr;
}

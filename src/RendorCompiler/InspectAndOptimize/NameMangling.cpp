#include "RendorCompiler/ASTInspection/ASTInspector.hpp"
#include <fmt/format.h>
#include <variant>

// For edef and foward statements
std::string ASTInspector::MangleName(const Body& FunctionArguments, std::string& Name, NodeType& ReturnType)
{
    std::string MangledName;
    if (!NameSpaces.empty())
    {
        MangledName = fmt::format("{}_{}->{}(", ReverseTypeTable.at(ReturnType), fmt::join(NameSpaces, "->"), Name);
    }
    else 
    {
        MangledName = fmt::format("{}_{}(", ReverseTypeTable.at(ReturnType), Name);
    }

    for (const auto& Node : FunctionArguments.ConnectedNodes)
    {
        MangledName += ReverseTypeTable.at(Node->Type) + " ";
    }
    
    // Remove extra space
    if (MangledName.back() == ' ')
    {
        MangledName.pop_back();
    }
    MangledName += ")";
    return MangledName;
}

std::string ASTInspector::MangleName(const std::vector<std::pair<std::string, NodeType>>& FunctionArguments, std::string& Name, NodeType& ReturnType)
{
    std::string MangledName;
    if (!NameSpaces.empty())
    {
        MangledName = fmt::format("{}_{}->{}(", ReverseTypeTable.at(ReturnType), fmt::join(NameSpaces, "->"), Name);
    }
    else 
    {
        MangledName = fmt::format("{}_{}(", ReverseTypeTable.at(ReturnType), Name);
    }

    // cppcheck-suppress unusedVariable
    for (const auto& [Arg, Type] : FunctionArguments)
    {
        MangledName += ReverseTypeTable.at(Type) + " ";
    }
    
    // Remove extra space
    if (MangledName.back() == ' ')
    {
        MangledName.pop_back();
    }
    MangledName += ")";
    return MangledName;
}
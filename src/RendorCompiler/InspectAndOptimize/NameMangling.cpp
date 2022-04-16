#include "RendorCompiler/ASTInspection/ASTInspector.hpp"
#include <fmt/format.h>
#include <variant>

// For edef and foward statements
std::string ASTInspector::MangleName(const std::variant<std::vector<std::pair<std::string, NodeType>>*, std::vector<NodeObject>*> FunctionArgumentsV, std::string& Name)
{
    std::string MangledName;
    if (!NameSpaces.empty())
    {
        MangledName = fmt::format("{}->{}(", fmt::join(NameSpaces, "->"), Name);
    }
    else 
    {
        MangledName = fmt::format("{}(", Name);
    }

    if (FunctionArgumentsV.index() == 0)
    {
        auto FunctionArguments = std::get<0>(FunctionArgumentsV);
        //cppcheck-suppress unusedVariable
        for (const auto& [ArgName, ArgType] : *FunctionArguments)
        {
            MangledName += ReverseTypeTable.at(ArgType) + " ";
        }
    }
    else 
    {
        auto FunctionArguments = std::get<1>(FunctionArgumentsV);
        for (const auto& ArgType : *FunctionArguments)
        {
            MangledName += ReverseTypeTable.at(ArgType->Type) + " ";
        }
    }
    
    // Remove extra space
    if (MangledName.back() == ' ')
    {
        MangledName.pop_back();
    }
    MangledName += ")";
    return MangledName;
}
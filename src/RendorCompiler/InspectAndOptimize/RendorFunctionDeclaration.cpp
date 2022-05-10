#include "RendorCompiler/ASTInspection/RendorFunctionDeclaration.hpp"
#include <algorithm>
#include <string_view>
#include <ranges>
#include <span>
#include <iomanip>

bool Declaration::IsFunc(NodeType& ReturnValue, std::vector<NodeType>& ArgsValue)
{
    // Check if the return value is not Null or Any
    if 
    ((ReturnValue != NodeType::Null) &&
    (ReturnValue != NodeType::Any)) 
    {
        if (ReturnValue != ReturnType)
        {
            return false;
        }
    }
    if (ArgsValue.size() == ArgTypes.size())
    {
        if (std::ranges::equal(
        std::span(ArgTypes.begin(), LastTypedArg), 
        std::span(ArgsValue.begin(), LastTypedArg)))
        {
            return true;
        }
    }
    return false;
}

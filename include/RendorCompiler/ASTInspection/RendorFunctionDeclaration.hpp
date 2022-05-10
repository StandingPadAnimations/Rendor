#ifndef RENDOR_FUNCTION_DECLARATION_HPP
#define RENDOR_FUNCTION_DECLARATION_HPP

#include <algorithm>
#define FALLTHROUGH [[fallthrough]]
#include <vector>
#include <string_view>
#include <string>

#include "RendorCompiler/Nodes/NodeType.hpp"

struct Declaration
{
    std::size_t LastTypedArg = 0;
    std::vector<NodeType> ArgTypes;
    NodeType ReturnType = NodeType::Any;
    std::string MangledName = "";
    std::string_view OriginalName = "";

    explicit Declaration(std::vector<NodeType>& Args) : ArgTypes(Args)
    {
        ArgTypes.reserve(5);
        for (std::size_t i = 0; i < ArgTypes.size(); ++i)
        {
            if (ArgTypes[i] == NodeType::Any)
            {
                LastTypedArg = i - 1;
            }
        }
    }

    explicit Declaration(std::vector<std::pair<std::string, NodeType>>& Args)
    {
        ArgTypes.reserve(5);
        for (std::size_t i = 0; i < Args.size(); ++i)
        {
            auto& [Name, Type] = Args[i];
            ArgTypes.push_back(Type);
            if (Type == NodeType::Any)
            {
                LastTypedArg = i - 1;
            }
        }
    }

    bool IsFunc(NodeType& ReturnValue, std::vector<NodeType>& ArgsValue);
};


#endif // RENDOR_OVERLOAD_CLASS
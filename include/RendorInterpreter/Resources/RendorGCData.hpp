#ifndef RENDOR_GD_DATA_HPP
#define RENDOR_GD_DATA_HPP

#include <vector>
#include <unordered_map>
#include <string_view>
#include <string>
#include <array>
using std::vector;
using std::unordered_map;
using std::equal_to;
using std::string;

#include "UnorderedMapLookUp.hpp"

namespace rengc
{
template <typename T>
class RendorGCData
{
    public:
        bool black_empty();
        bool grey_empty();
        bool white_empty();
        void promote(std::string_view Object);
        void promote_grey();
        void demote(std::string_view Object);
        void add(std::string_view ObjectName, T Object);
        void clean();

    private:
        std::unordered_map<std::string, T, string_hash, std::equal_to<>> m_BlackObjects;
        std::unordered_map<std::string, T, string_hash, std::equal_to<>> m_GreyObjects;
        std::unordered_map<std::string, T, string_hash, std::equal_to<>> m_WhiteObjects;
};


template <typename T, size_t m_size>
class RendorStack
{
    public:
        size_t size();
        bool empty();
        void push(T Object);
        void pop();
        &T top();

    private:
        size_t m_Size = 0; // Size 
        T& m_Top; // Top element
        std::array<T, m_size> m_Stack;
};
}

#endif // RENDOR_GD_DATA_HPP
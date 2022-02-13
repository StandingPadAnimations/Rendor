#ifndef UNORDERED_MAP_LOOKUP_HPP
#define UNORDERED_MAP_LOOKUP_HPP
#include <string_view>
#include <string>
#include <functional>
#include <cstdint>

using std::hash;
using std::string_view;
using std::string;


// Code is from here: https://www.cppstories.com/2021/heterogeneous-access-cpp20/
struct string_hash {
    using is_transparent = void;
    [[nodiscard]] size_t operator()(const char *txt) const {
        return std::hash<std::string_view>{}(txt);
    }
    [[nodiscard]] size_t operator()(std::string_view txt) const {
        return std::hash<std::string_view>{}(txt);
    }
    [[nodiscard]] size_t operator()(const std::string &txt) const {
        return std::hash<std::string>{}(txt);
    }
};

#endif // UNORDERED_MAP_LOOKUP_HPP
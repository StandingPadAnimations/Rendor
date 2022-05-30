#ifndef RENDOR_TYPES_HPP
#define RENDOR_TYPES_HPP
#include <atomic>
#include <cstdint>

enum class Type : std::uint8_t
{
    VOID,
    INT,
    DOUBLE,
    STRING,
    BOOl
};

#endif // RENDOR_TYPES_HPP
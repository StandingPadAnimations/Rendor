#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define FALLTHROUGH [[fallthrough]]

#include <cstdint>
#include <variant>
#include <string>

using RendorInt = std::int64_t;
using RendorDouble = double;
using RendorVariant = std::variant<RendorInt, RendorDouble, std::string, bool>;
using RendorMathVariant = std::variant<RendorInt, RendorDouble>;
/*
    * REFERENCE
    * reg - register
    * reg ref - a single bit that defines if a register is being refered or not
*/
enum class ByteCodeEnum : std::uint16_t
{
    /* --------------------------------- Scopes --------------------------------- */
    ALLOC,          //* n amount of registers
    FREE,

    /* ----------------------------- Create objects ----------------------------- */
    MOVE_I,         //* reg, reg ref, int64  or reg
    MOVE_D,         //* reg, reg ref, double or reg
    MOVE_S,         //* reg, reg ref, string or reg
    MOVE_B,         //* reg, reg ref, bool   or reg

    /* ---------------------------------- Math ---------------------------------- */
    ADD,            //* reg, reg ref, value or reg
    SUB,            //* reg, reg ref, value or reg
    MUL,            //* reg, reg ref, value or reg
    DIV,            //* reg, reg ref, value or reg

    /* -------------------------- Conditional Operators ------------------------- */
    EQUAL,          //* reg, reg ref, value or reg
    NOT_EQUAL,      //* reg, reg ref, value or reg
    GREATER,        //* reg, reg ref, value or reg
    LESS,           //* reg, reg ref, value or reg
    GREATER_EQUAL,  //* reg, reg ref, value or reg
    LESS_EQUAL,     //* reg, reg ref, value or reg
};

enum class ConstType
{
    NONE,
    RENDOR_INT,
    RENDOR_DOUBLE,
    STRING,
    BOOL,
};

#endif // DEFINITIONS_HPP
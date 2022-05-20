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
    * size - u32: string size
    * reg - u32: register
    * reg ref - a single bit that defines if a register is being refered or not
    * GoL - a single bit telling if the second value is a global reg/const pool (0) or a local reg/value (1)
*/
enum class ByteCodeEnum : std::uint8_t
{
    /* --------------------------------- Scopes --------------------------------- */
    alloc,          //* n amount of registers
    free,

    /* -------------------------------- Functions ------------------------------- */
    define,         //* reg
    call,           //* reg

    /* ------------------------------ Copy objects ------------------------------ */
    l_mov_i,        //* reg, reg ref, GoL, int64  or reg
    l_mov_d,        //* reg, reg ref, GoL, double or reg
    l_mov_s,        //* reg, reg ref, GoL, string or reg
    l_mov_b,        //* reg, reg ref, GoL, bool   or reg

    g_mov_i,        //* reg, reg ref, GoL, int64  or reg
    g_mov_d,        //* reg, reg ref, GoL, double or reg
    g_mov_s,        //* reg, reg ref, GoL, string or reg
    g_mov_b,        //* reg, reg ref, GoL, bool   or reg

    /* ---------------------------------- Math ---------------------------------- */
    add,            //* reg, reg ref, GoL, value or reg
    sub,            //* reg, reg ref, GoL, value or reg
    mul,            //* reg, reg ref, GoL, value or reg
    div,            //* reg, reg ref, GoL, value or reg

    /* -------------------------- Conditional Operators ------------------------- */
    eq,             //* reg, reg ref, GoL, value or reg
    eq_not,         //* reg, reg ref, GoL, value or reg
    gr,             //* reg, reg ref, GoL, value or reg
    less,           //* reg, reg ref, GoL, value or reg
    gr_eq,          //* reg, reg ref, GoL, value or reg
    less_eq,        //* reg, reg ref, GoL, value or reg
};

enum class GVT_GFT_Enum : std::uint8_t 
{
    /* --------------------------------- Modules -------------------------------- */
    import,         //* size, path
    remove,         //* size, module
    /* -------------------------------- GFT & GVT ------------------------------- */
    gvt_define,     //* reg, size, string
    gft_define,     //* reg, size, string

    gvt_resolve,    //* reg, size, string
    gft_resolve,    //* reg, size, string
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
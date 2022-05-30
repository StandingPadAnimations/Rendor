#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define FALLTHROUGH [[fallthrough]]

#include <cstdint>
#include <variant>
#include <string>

using RendorInt = std::int64_t;
using RendorDouble = double;
using RendorVariant = std::variant<std::string, bool>;
using RendorMathVariant = std::variant<RendorInt, RendorDouble>;

/*
    * REFERENCE
    * reg - u16: register
    * reg ref - u8: determines the type of the second register
    * {
    *   0 - Const Pool
    *   1 - Global
    *   2 - Last Stack Frame
    *   3 - Local
    * }
    * ret - u8: determines if there is a return value:
    * {
    *   0 - No return value
    *   1 - Return Value
    * }  
    * func-o-var - u8: for resolving and exporting names
    * {
    *   0 - function
    *   1 - variable
    * }
*/
enum class ByteCodeEnum : std::uint8_t
{
    /* --------------------------------- Scopes --------------------------------- */
    alloc,          //* Size,   NULL16, NULL16, NULL8, NULL8, NULL8
    free,           //* NULL16, NULL16, NULL16, NULL8, NULL8, NULL8

    /* -------------------------------- Functions ------------------------------- */
    call,           //* return reg, reg for function, ret, NULL8, NULL8

    /* -------------------------------- Variables ------------------------------- */
    mov,            //* reg1, reg2,   NULL16, reg-ref1, reg-ref2, ref-o-not
    mov_n,          //* reg1, NULL16, NULL16, reg-ref,  NULL8,    NULL8

    /* ---------------------------------- jump ---------------------------------- */
    jmp,            //* jump-val, forward-o-back

    /* ---------------------------------- Math ---------------------------------- */
    add,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    sub,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    mul,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    div,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    pow,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3

    /* -------------------------- Conditional Operators ------------------------- */
    eq,             //* reg1, reg2, NULL16, reg-ref1, reg-ref2, NULL8
    eq_not,         //* reg1, reg2, NULL16, reg-ref1, reg-ref2, NULL8
    gr,             //* reg1, reg2, NULL16, reg-ref1, reg-ref2, NULL8
    less,           //* reg1, reg2, NULL16, reg-ref1, reg-ref2, NULL8
    gr_eq,          //* reg1, reg2, NULL16, reg-ref1, reg-ref2, NULL8
    less_eq,        //* reg1, reg2, NULL16, reg-ref1, reg-ref2, NULL8
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
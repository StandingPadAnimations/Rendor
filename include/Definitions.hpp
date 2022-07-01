#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define FALLTHROUGH [[fallthrough]]

#ifdef __GNUC__
#   define MAKE_STRING(x)  #x
#   define DIAGNOSTIC_ERROR(w) _Pragma(MAKE_STRING(GCC diagnostic error w))
#   define DIAGNOSTIC_IGNORE(w) _Pragma(MAKE_STRING(GCC diagnostic ignored  w))
#   define DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#   define DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#elif __clang__
#   define MAKE_STRING(x)  #x
#   define DIAGNOSTIC_ERROR(w) _Pragma(MAKE_STRING(GCC diagnostic error w))
#   define DIAGNOSTIC_IGNORE(w) _Pragma(MAKE_STRING(GCC diagnostic ignored  w))
#   define DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#   define DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#else
#   define DIAGNOSTIC_ERROR(w)
#   define DIAGNOSTIC_IGNORE(w)
#   define DIAGNOSTIC_PUSH
#   define DIAGNOSTIC_POP
#endif 

#include <cstdint>
#include <variant>
#include <string>
#include <vector>

#include "Exceptions.hpp"

struct Constant;
struct Function;

using RendorInt       = std::int64_t;
using RendorDouble    = double;
using RendorConst     = std::variant<std::string, bool>;
using RendorVec       = std::vector<Constant>;
using RendorReference = Constant*;
using RendorNum       = std::variant<RendorInt, RendorDouble>;

#define RENDOR_SEG_FAULT error::RendorException("Segmentation Fault")
/*
    * REFERENCE
    * size - u8: the amount of registers
    * reg - u16: register
    * reg-ref - u8: determines the type of the second register
    * {
    *   0 - Const Pool
    *   1 - Global
    *   2 - Last Stack Frame
    *   3 - Local
    * }
    * Loc-of-Index - u8: determines the location of an index
    * {
    *   0 - Const Pool
    *   1 - Global
    *   2 - Last Stack Frame
    *   3 - Local
    * }
    * jmp-options - u8: affects how jmp works
    * {
    *   0 - in current function
    *   1 - with current module
    * }
    * free-options - u8: affects how free works
    * {
    *   0 - Call stack
    *   1 - Module stack
    * }
    * ref-type - u8: affects how a value can be read
    * {
    *   0 - nomal 
    *   1 - dereferenced
    * }
*/
enum class ByteCodeEnum : std::uint8_t
{
    /* --------------------------------- Scopes --------------------------------- */
    alloc,          //* NULL64, size
    free,           //* NULL64, free-options
    mod_push,       //* mod-index, NULL8

    /* -------------------------- Register instructions ------------------------- */
    mov,            //* reg1, reg2,   ref-type1, ref-type2, reg-ref1, reg-ref2, NULL8
    cpy,            //* reg1, reg2,   ref-type1, ref-type2, reg-ref1, reg-ref2, NULL8
    ref,            //* reg1, reg2,   ref-type1, ref-type2, reg-ref1, reg-ref2, NULL8

    /* --------------------------- Array instructions --------------------------- */
    movElm,         //* reg1, reg2, index, reg-ref1, reg-ref2, Loc-of-Index
    cpyElm,         //* reg1, reg2, index, reg-ref1, reg-ref2, Loc-of-Index
    refElm,         //* reg1, reg2, index, reg-ref1, reg-ref2, Loc-of-Index

    /* ---------------------------------- jump ---------------------------------- */
    jmp,            //* index, jmp-options
    
    /* ---------------------------------- Math ---------------------------------- */
    add,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    sub,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    mul,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    div,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    pow,            //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3

    icr,            //* reg1, reg-ref1
    dcr,            //* reg1, reg-ref1

    /* -------------------------- Conditional Operators ------------------------- */
    eq,             //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    eq_not,         //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    gr,             //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    less,           //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    gr_eq,          //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
    less_eq,        //* reg1, reg2, reg3, reg-ref1, reg-ref2, reg-ref3
};

enum class ConstType
{
    NONE,
    CONST_NUM,
    CONST_STR,
    CONST_BOOL,
    CONST_REF,
    CONST_VEC,
};

// Code from Rythm#6156 with slight changes 
inline RendorInt Rythm_RendorPow(RendorInt& x, RendorInt& y){
    RendorInt ret = 1;
    for(; y; y >>= 1){
        if(y & 1)
        {
            ret *= x;
        } 
        x *= x;
    }
    return ret;
}

#endif // DEFINITIONS_HPP
#ifndef BYTECODE_ENUM_HPP
#define BYTECODE_ENUM_HPP

enum class ByteCodeEnum
{
    /* ------------------------------- Load Scope ------------------------------- */
    LOAD,
    END,
    CIMPORT,
    IMPORT,

    /* -------------------------------- Constants ------------------------------- */
    CONST_OP,
    ASSIGN,
    
    /* -------------------------------- Functions ------------------------------- */
    DEFINE,
    FINALIZE_CALL,
    FUNCTION,

    /* --------------------------------- If-Else -------------------------------- */
    OPERATOR,
    JMP,
    ENDIF,
};

#endif // BYTECODE_ENUM_HPP
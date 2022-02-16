#ifndef BYTECODE_ENUM_HPP
#define BYTECODE_ENUM_HPP

enum class ByteCodeEnum
{
    /* ------------------------------- Load Scope ------------------------------- */
    LOAD,

    /* -------------------------------- Constants ------------------------------- */
    CONST_OP,
    ASSIGN,
    ARGUMENT,
    
    /* -------------------------------- Functions ------------------------------- */
    DEFINE,
    CALL,
    FINALIZE_CALL,
    FUNCTION,

    /* --------------------------------- If-Else -------------------------------- */
    OPERATOR,
    JMP_IF_FALSE,
    ENDIF,
};

#endif // BYTECODE_ENUM_HPP
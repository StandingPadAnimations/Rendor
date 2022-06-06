#ifndef IR_DEFINITIONS_HPP
#define IR_DEFINITIONS_HPP

enum class IR_def 
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    ICR,
    DCR,

    MOV,
    MOV_N,
    CPY,
    REF,

    STK_MOV,
    STK_MOV_N,
    STK_CPY,

    EQ,
    NOT_EQ,
    GR,
    LESS,
    GR_EQ,
    LESS_EQ,
};

enum class IR_Type 
{
    INT,
    DOUBLE,
    STRING,
    BOOL,
    REFERENCE,
    VOID,
};

#endif // IR_DEFINITIONS_HPP
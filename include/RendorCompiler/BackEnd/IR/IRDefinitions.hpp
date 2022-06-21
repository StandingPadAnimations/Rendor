#ifndef IR_DEFINITIONS_HPP
#define IR_DEFINITIONS_HPP

namespace RendorIR
{
    inline namespace v1_0_0
    {
        enum class IR_def 
        {
            ADD,
            SUB,
            MUL,
            DIV,
            POW,
            ICR,
            DCR,

            ASSIGN,

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
            RET,
        };

        enum class ManipulationType 
        {
            REF,
            MOV,
            CPY
        };
    }
}
#endif // IR_DEFINITIONS_HPP
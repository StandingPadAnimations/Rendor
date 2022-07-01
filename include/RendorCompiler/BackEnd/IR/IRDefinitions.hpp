#ifndef IR_DEFINITIONS_HPP
#define IR_DEFINITIONS_HPP

namespace RendorIR
{
    inline namespace v1_0_0
    {
        enum class IR_def 
        {
            CONSTANT,
            ASSIGN_VARIABLE
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
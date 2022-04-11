#ifndef TEMP_ID_HPP
#define TEMP_ID_HPP

enum class TempID
{
    None,

    // Definitions
    ArithAssemble,
    IdentifierDefinition,
    VariableDefition,
    
    // Imports and Exports 
    Import,
    Export,

    // Function things
    FowardDefinition,
    FunctionCall,
    FunctionScope,
    FunctionDefiniton,
    FunctionArgumentsDefinition,
    FowardArgsDefinition,
    NameSpace,
    NameSpaceCall,
    Extern,

    // If-else statements 
    IfStatementDefinition,
    ElseDefinition,
    ConditionDefinition,
    IfElseScope
};

#endif // TEMP_ID_HPP
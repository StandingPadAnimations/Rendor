#ifndef IR_VARIABLES_IPP
#define IR_VARIABLES_IPP

#ifndef IR_CONTEXT_HPP
#error __FILE__ should only be included from IRContext.hpp.
#endif // IR_CONTEXT_HPP

#include "RendorCompiler/BackEnd/IR/IRContext.hpp" // should be fine since we check if this is defined before including


template <typename AssignmentType>
void RendorIR::IR_Context::CreateVariable(const std::string& Name, IR_Type Type, AssignmentType Val);
template <typename AssignmentType>
void RendorIR::IR_Context::CreateGlobalVariable(const std::string& Name, IR_Type Type, AssignmentType Val);


#endif // IR_VARIABLES_IPP
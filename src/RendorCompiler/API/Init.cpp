#include "RendorAPI/FunctionTable.hpp"

void Init(const RendorMethod *MethodList){
    size_t Index = 0;
    do 
    {
        RendorMethod Method = MethodList[Index];
        if (Method.Name == NULL)
        {
            break;
        }
        else if (Method.RendorFunc == NULL)
        {
            throw std::runtime_error("Can't have Null function pointer");
        }
        
        FunctionTable::Functions[Method.Name] = Method.RendorFunc;
        FunctionTable::FunctionsArgCount[Method.Name] = Method.ArgCount;
        ++Index;
    } while (true);
}
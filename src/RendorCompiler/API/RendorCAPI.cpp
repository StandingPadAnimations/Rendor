#include "RendorCompiler/Parser/Parser.hpp"

void Parser::InitModule(RendorMethod *MethodList)
{
    bool IterateArray = false;
    size_t Index = 0;
    do 
    {
        RendorMethod Method = MethodList[Index];
        if (Method.Name == NULL)
        {
            IterateArray = false;
        }

        CppFunctions.emplace(Method.Name, Method.RendorFunc);
    } while (IterateArray);
}
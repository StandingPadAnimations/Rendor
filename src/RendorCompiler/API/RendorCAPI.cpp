#include "RendorCompiler/ASTInspection/ASTInspector.hpp"
#include "RendorAPI/RendorAPI.hpp"

// cppcheck-suppress constParameter
bool ASTInspector::InitModule(RendorMethod* MethodList)
{
    if (!MethodList)
    {
        return false;
    }

    size_t Index = 0;
    do 
    {
        RendorMethod Method = MethodList[Index];
        if (Method.Name == NULL)
        {
            break;
        }
        RendorEngineCompiler::EngineContext.FunctionTable.emplace(Method.Name, Method.ArgCount);
        ++Index;
    } while (true);
    return true;
}
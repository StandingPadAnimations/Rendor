#include "RendorInterpreter/Interpreter.hpp"

void Interpreter::AddModule()
{
    Modules.push_back(Module{});
    CurrentModule = &Modules.back();
    CurrentModule->ConstPool = ConstPool{};
    CurrentModule->GlobalVars = StackFrame{};
}
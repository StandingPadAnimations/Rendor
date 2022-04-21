#include <iostream>
#include <string>
#include "RendorAPI/RendorAPI.hpp"

void RENDOR_ECHO_FUNCTION(RendorState* R)
{
    if (R->Rendor_ConstEval())
    {
        return;
    }
    const char* PrintValue = R->Rendor_GrabConstFromStackAsConstChar();
    std::cout << PrintValue << "\n";
}

void RENDOR_INPUT_FUNCTION(RendorState* R)
{
    if (R->Rendor_ConstEval())
    {
        return;
    }
    const char* StringToPrint = R->Rendor_GrabConstCharFromStack();
    std::string Input;
    std::cout << StringToPrint;
    std::getline(std::cin, Input);
    R->Rendor_DropConstCharOnStack(Input.c_str());
}

static RendorMethod Rendor_IO_InitList[] = {
    {"echo", 1, RENDOR_ECHO_FUNCTION},
    {"input", 1, RENDOR_INPUT_FUNCTION},
    {NULL, 0, NULL}
};

RENDOR_INIT Rendor_Module_Rendor_IO(RendorState* R)
{
    R->Rendor_InitModule(Rendor_IO_InitList);
}

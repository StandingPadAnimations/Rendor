#include <iostream>
#include <string>
#include "RendorSTD/IO.hpp"

void RENDOR_ECHO_FUNCTION()
{
    const char* PrintValue = Rendor_GrabConstFromStackAsConstChar();
    std::cout << PrintValue << "\n";
}

void RENDOR_INPUT_FUNCTION()
{
    const char* StringToPrint = Rendor_GrabConstCharFromStack();
    std::string Input;
    std::cout << StringToPrint;
    std::getline(std::cin, Input);
    Rendor_DropConstCharOnStack(Input.c_str());
}

static RendorMethod Rendor_IO_InitList[] = {
    {"echo", 1, RENDOR_ECHO_FUNCTION},
    {"input", 1, RENDOR_INPUT_FUNCTION},
    {NULL, 0, NULL}
};

RENDOR_INIT RendorVM_Module_IO()
{
    RendorVM_InitModule(Rendor_IO_InitList);
}

RENDOR_INIT RendorC_Module_IO()
{
    RendorC_InitModule(Rendor_IO_InitList);
}

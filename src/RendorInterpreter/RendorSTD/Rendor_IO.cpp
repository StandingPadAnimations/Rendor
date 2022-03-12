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
};

RENDOR_INIT RendorModule_IO()
{
    Rendor_InitModule(Rendor_IO_InitList, 2);
}

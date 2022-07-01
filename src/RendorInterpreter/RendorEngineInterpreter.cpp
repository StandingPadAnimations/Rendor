#include "RendorInterpreter/Interpreter.hpp"
#include "RendorEngine.hpp"

void RendorEngineInterpreter::run(const std::string File) noexcept
{
    binary_io::file_istream Stream{File};
    Interpreter Instance{Stream};

    if (!Instance.PrepareInterpreter())
    {
        Instance.ThrowStackTrace();
        return;
    }
    try
    {
        Instance.ExecuteMain();
    }
    catch (std::exception& exp)
    {
        std::cout << exp.what() << std::endl;
        return;
    }
    catch (...)
    {
        std::cout << "Unknown Error Occured" << std::endl;
        return;
    }
}
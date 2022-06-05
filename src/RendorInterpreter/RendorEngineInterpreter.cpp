#include "RendorInterpreter/Interpreter.hpp"
#include "RendorEngine.hpp"

void RendorEngineInterpreter::run(const std::string File)
{
    binary_io::file_istream Stream{File};
    Interpreter Instance{Stream};

    if (!Instance.PrepareInterpreter())
    {
        Instance.ThrowStackTrace();
    }
    Instance.ExecuteMain();
}
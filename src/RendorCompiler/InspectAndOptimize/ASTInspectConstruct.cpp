#include "RendorCompiler/ASTInspection/ASTInspector.hpp"
#include "RendorCompiler/RendorCompilerState.hpp"
#include <memory>

ASTInspector::ASTInspector()
{
    CurrentPath = boost::dll::program_location().parent_path().parent_path().string() + "/RendorSTD";
    RendorEngineCompilerState = std::make_unique<CompilerState>();
}
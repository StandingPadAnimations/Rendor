#include "RendorCompiler/ASTInspection/ASTInspector.hpp"
#include <memory>

ASTInspector::ASTInspector()
{
    CurrentPath = boost::dll::program_location().parent_path().parent_path().string() + "/RendorSTD";
    RendorEngineCompilerState.Rendor_InitModule = Init;
}
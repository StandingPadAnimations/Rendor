#include "RendorCompiler/RendorDeltaOptimizer.hpp"

void RendorDeltaOptimizer::DeltaOptimizer(std::vector<std::string>& ByteCode)
{
    if (RendorEngineCompiler::OptimizeByteCode)
    {
        
    }
    ByteCode.erase(std::remove(
    ByteCode.begin(), 
    ByteCode.end(), 
    std::string("")
    ));
}
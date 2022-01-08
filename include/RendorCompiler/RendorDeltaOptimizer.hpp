#ifndef RENDOR_DELTA_OPTIMIZER_HPP
#define RENDOR_DELTA_OPTIMIZER_HPP

#include <vector>
#include <string>

#include "RendorCompiler/MathEvaluator.hpp"
#include "RendorCompiler/Nodes.hpp"

class RendorDeltaOptimizer
{
    public:
        static void DeltaOptimizer(std::vector<std::string>& ByteCode);
};

#endif // RENDOR_DELTA_OPTIMIZER_HPP
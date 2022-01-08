#include "RendorCompiler/RendorDeltaOptimizer.hpp"

void RendorDeltaOptimizer::DeltaOptimizer(std::vector<std::string>& ByteCode)
{
    std::string SavedString = "";
    for (auto ByteCodeOp = ByteCode.begin(); ByteCodeOp != ByteCode.end();)
    {
        size_t ByteCodeSpaceIndex  = ByteCodeOp->find_first_of(" ");
        std::string_view Command  (ByteCodeOp->begin(), ByteCodeOp->begin() + ByteCodeSpaceIndex);
        std::string_view Args     (ByteCodeOp->begin() + (ByteCodeSpaceIndex + 1), ByteCodeOp->end());

        if (Command == "CONST")
        {
            if 
            ((Args[0] == '_') &&
            (Args[1] == '$'))
            {
                SavedString = *(ByteCodeOp + 1);
                *ByteCodeOp = "";
                *(ByteCodeOp + 1) = "";
            }
            // If it's a static operation, it can be optimized 
            else if (Args.find_first_not_of("1234567890.()^*/+- "))
            {
                // Solve operation
                std::string EvaluatedConstInstruction = PostFixEval(Args);
                *ByteCodeOp = "CONST " + EvaluatedConstInstruction; // Replace const instruction
            }
        }
        else if (Command == "FINALIZE_CALL")
        {
            if (SavedString.size())
            {
                ByteCode.insert(ByteCodeOp + 1, SavedString);
                SavedString = "";
            }
        }
        ++ByteCodeOp;
    }

    ByteCode.erase(std::remove(
        ByteCode.begin(), 
        ByteCode.end(), 
        std::string("")
    ));
}
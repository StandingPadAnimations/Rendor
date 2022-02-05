#include "RendorCompiler/RendorDeltaOptimizer.hpp"

void RendorDeltaOptimizer::DeltaOptimizer(std::vector<std::string>& ByteCode)
{
    for (size_t Op = 0; Op < ByteCode.size(); ++Op)
    {
        ByteCodeArray SplitedByteCodeOp = SplitByteCode(ByteCode[Op]);
        std::string_view Command = SplitedByteCodeOp[0];
        std::string_view Args = SplitedByteCodeOp[1];

        if (Command == "CONST")
        {
            if 
            ((Args[0] == '&') &&
            (Args[1]  == '_') &&
            (Args[2]  == 'A'))
            {
                // Solve operation
                std::string ActualConstant{Args.begin() + 3, Args.end()};
                std::string EvaluatedConstInstruction = PostFixEval(ActualConstant);
                ByteCode[Op] = "CONST " + EvaluatedConstInstruction; // Replace const instruction
            }

            SplitedByteCodeOp = SplitByteCode(ByteCode[Op]);
            Command = SplitedByteCodeOp[0];
            Args = SplitedByteCodeOp[1];

            ByteCodeArray NextByteCodeOp = SplitByteCode(ByteCode[Op+1]);
            if (NextByteCodeOp[0] == "CONST")
            {
                std::string NewConst = (boost::format("CONST (%s,%s)") % std::string{Args} % std::string{NextByteCodeOp[1]}).str();
                ByteCode[Op] = NewConst;
                ByteCode[Op+1] = "";
            }
        }
    }

    ByteCode.erase(std::remove(
        ByteCode.begin(), 
        ByteCode.end(), 
        std::string("")
    ));
}
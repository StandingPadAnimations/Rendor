#ifndef BYTECODE_GENERATOR_HPP
#define BYTECODE_GENERATOR_HPP

#include <vector>
#include <string>

#include "RendorCompiler/Math/MathEvaluator.hpp"
#include "RendorCompiler/Nodes/NodeType.hpp"


enum class ByteCodeType
{
    INT32,
    INT64,
    DOUBLE,
    STRING,
    BOOL,
    ARITH,
    REFERENCE
};

class RendorByteCodeGenerator
{
    public:
        /* -------------------------------------------------------------------------- */
        /*                            Generation functions                            */
        /* -------------------------------------------------------------------------- */

        /* ------------------------------ Global Scope ------------------------------ */
        void CreateGlobal();
        void EndGlobal();

        /* --------------------------------- Consts --------------------------------- */
        void CreateVariable(const std::string& Name);
        void CreateConst(ByteCodeType Type, const std::string& Value);

        /* -------------------------------- Functions ------------------------------- */
        void CreateFunc(const std::string& Name);
        void CreateFuncEnd();
        void CallFunction(const std::string& Name);

        /* ---------------------------------- Logic --------------------------------- */
        void CreateLogic(const std::string& Operator);
        void CreateJMP(int32_t JMPAmount);
        void CreateEndIf();

        /* --------------------------- Increment/Decrement -------------------------- */
        void CreateIncr(const std::string& Name);
        void CreateDecr(const std::string& Name);

        /* ----------------------- Index of current operation ----------------------- */
        size_t GetIndex();
        std::vector<std::string> ByteCode;
};

#endif // BYTECODE_GENERATOR_HPP
#ifndef BYTECODE_GENERATOR_HPP
#define BYTECODE_GENERATOR_HPP

#include <vector>
#include <string>

#include "RendorCompiler/Math/Mathparsing.hpp"
#include "RendorCompiler/Math/MathEvaluator.hpp"
#include "RendorCompiler/Nodes/NodeType.hpp"
#include "RendorCompiler/BackEnd/ByteCodeType.hpp"
#include "RendorCompiler/BackEnd/ByteCodeContext.hpp"


struct RendorByteCodeGenerator
{
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

    /* --------------------------------- C++ API -------------------------------- */
    void RedefineCppName(const std::string& OriginalName, const std::string& NewName);

    /* ---------------------------------- Logic --------------------------------- */
    void CreateLogic(const std::string& Operator);
    void CreateJMP(int32_t JMPAmount);
    void CreateEndIf();

    /* --------------------------- Increment/Decrement -------------------------- */
    void CreateIncr(const std::string& Name);
    void CreateDecr(const std::string& Name);

    /* ------------------------------- Attributes ------------------------------- */
    void CreateAttribute(std::string_view Attr);

    /* --------------------------------- Imports -------------------------------- */
    void CreateCppImport(const std::string& ModuleName);

    /* ----------------------- Index of current operation ----------------------- */
    size_t GetIndex();
    
    explicit RendorByteCodeGenerator(RendorContext* Context, std::vector<std::string>* ByteCodeVector) : 
    ByteCode(ByteCodeVector), 
    ByteCodeContext(Context){}

    std::vector<std::string>* ByteCode;
    RendorContext* ByteCodeContext;
};

#endif // BYTECODE_GENERATOR_HPP
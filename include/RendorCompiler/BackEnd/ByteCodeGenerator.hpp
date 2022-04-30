#ifndef BYTECODE_GENERATOR_HPP
#define BYTECODE_GENERATOR_HPP

#include <vector>
#include <string>
#include <string_view>
#include <bit>
#include <bitset>
#include <ios>
#include <map>

#include "RendorCompiler/Math/Mathparsing.hpp"
#include "RendorCompiler/Math/MathEvaluator.hpp"
#include "RendorCompiler/Nodes/NodeType.hpp"
#include "RendorCompiler/BackEnd/ByteCodeType.hpp"
#include "RendorCompiler/BackEnd/ByteCodeContext.hpp"
#include "RendorCompiler/BackEnd/ByteCodeEnum.hpp"

#include <boost/endian/conversion.hpp>
#include <boost/endian/buffers.hpp>

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

        /* ---------------------------- Compile function ---------------------------- */
        void CompileByteCode(const std::string& FilePath);
        
        explicit RendorByteCodeGenerator(RendorContext* Context, std::vector<std::string>* ByteCodeVector) : 
        ByteCode(ByteCodeVector), 
        ByteCodeContext(Context){}

        std::vector<std::string>* ByteCode;
        RendorContext* ByteCodeContext;

    private:
        /* --------------------------------- Output --------------------------------- */
        std::ofstream CrenOutput;
        const std::map<std::string_view, ByteCodeEnum, std::less<>> ByteCodeMapping
        {
            {"LOAD",            ByteCodeEnum::LOAD},
            {"END",             ByteCodeEnum::END},
            {"CONST",           ByteCodeEnum::CONST_OP},
            {"ASSIGN",          ByteCodeEnum::ASSIGN},
            {"ARGUMENT",        ByteCodeEnum::ASSIGN},
            {"DEFINE",          ByteCodeEnum::DEFINE},
            {"FINALIZE_CALL",   ByteCodeEnum::FINALIZE_CALL},
            {"FUNCTION",        ByteCodeEnum::FUNCTION},
            {"OPERATOR",        ByteCodeEnum::OPERATOR},
            {"JMP",             ByteCodeEnum::JMP},
            {"ENDIF",           ByteCodeEnum::ENDIF},
            {"CIMPORT",         ByteCodeEnum::CIMPORT},
            {"IMPORT",          ByteCodeEnum::IMPORT},
        };

        const std::map<std::string_view, ConstType, std::less<>> TypeMapping
        {
            {"INT64",           ConstType::INT64},
            {"DOUBLE",          ConstType::DOUBLE},
            {"STRING",          ConstType::STRING},
            {"BOOL",            ConstType::BOOL},
            {"ARITHMETHIC",     ConstType::ARITHMETHIC},
            {"REFERENCE",       ConstType::REFERENCE},
        };

        /* --------------------------- repeated operations -------------------------- */
        void WriteRendorOp(const std::string& Operation);
        void WriteString(const std::string& String);
        /* --------------------------------- Header --------------------------------- */
        void CompileHeader();

        /* -------------------------------- Constants ------------------------------- */
        void CompileInt64(boost::endian::little_int64_buf_t Int64Value);
        void CompileDouble(boost::endian::little_float64_buf_t DoubleValue);
        void CompileString(const std::string& StringValue);
        void CompileBool(boost::endian::little_int32_buf_at BoolValue);
        void CompileReference(const std::string& Reference);

        /* --------------------------- User defined things -------------------------- */
        void CompileFunc(const std::string& Name);
        void CompileFuncCall(const std::string& Name);
        void CompileVar(const std::string& Name);
};



#endif // BYTECODE_GENERATOR_HPP
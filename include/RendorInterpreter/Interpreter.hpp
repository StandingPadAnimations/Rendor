#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <vector>
#include <array>
#include <map>
#include <fstream>
#include <algorithm>
#include <functional>
#include <optional>
#include <memory>
#include <variant>
#include <stack>
#include <tuple>

// Other Parts of the Rendor Engine Interpreter 
#include "RendorInterpreter/RendorTypes.hpp"
#include "RendorInterpreter/VariableType.hpp"
#include "RendorInterpreter/RendorDefinitions.hpp"
#include "RendorInterpreter/CodeObjects.hpp"
#include "RendorInterpreter/ByteCodeEnum.hpp"

#include "RendorAPI/RendorAPI.h"

#include "Exceptions.hpp"
#include "UnorderedMapLookUp.hpp"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/tokenizer.hpp>

enum class ConstType
{
    INT64,
    DOUBLE,
    STRING,
    BOOL,
    ARITHMETHIC,
    REFERENCE
};


class Interpreter
{
    public:
        static void ExecuteByteCode(const boost::interprocess::mapped_region& File);
        static void DisposeConst(TypeObject RendorConstID);

        /* ------------------------------ API functions ----------------------------- */
        static int64_t GrabInt64FromStack();
        static double GrabDoubleFromStack();
        static const char* GrabConstCharFromStack();
        static const char* GrabConstFromStackAsConstChar();
        static bool GrabBoolFromStack();
        static VariableType GrabObjTypeFromStack();

        static void DropInt64OnStack(int64_t Const);
        static void DropDoubleOnStack(double Const);
        static void DropConstCharOnStack(const char* Const);
        static void DropBoolOnStack(bool Const);
        static void PopStack();

        static void InitModule(RendorMethod *MethodList);

    private:
        /* -------------------------------------------------------------------------- */
        /*                            Interpreter Internals                           */
        /* -------------------------------------------------------------------------- */
        /* -------------------------------- Bytecode -------------------------------- */
        inline static const std::unordered_map<std::string, ByteCodeEnum, string_hash, std::equal_to<>> ByteCodeMapping
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

        inline static const std::unordered_map<std::string, Operator, string_hash, std::equal_to<>> OperatorMapping
        {
            {"EQUAL",               Operator::EQUAL},
            {"NOT_EQUAL",           Operator::NOT_EQUAL},
            {"GREATER_THAN",        Operator::GREATER_THAN},
            {"GREATER_OR_EQUAL",    Operator::GREATER_OR_EQUAL},
            {"LESS_THAN",           Operator::LESS_THAN},
            {"LESS_OR_EQUAL",       Operator::LESS_OR_EQUAL},
        };

        inline static const std::unordered_map<std::string, ConstType, string_hash, std::equal_to<>> TypeMapping
        {
            {"INT64",           ConstType::INT64},
            {"DOUBLE",          ConstType::DOUBLE},
            {"STRING",          ConstType::STRING},
            {"BOOL",            ConstType::BOOL},
            {"ARITHMETHIC",     ConstType::ARITHMETHIC},
            {"REFERENCE",       ConstType::REFERENCE},
        };


        /* ----------------------------- Rendor's Memory ---------------------------- */
        inline static std::unordered_map<std::string, std::unique_ptr<Function>, string_hash, std::equal_to<>> UserDefinedFunctions; // Stores index of user defined functions in the main file

        /* ------------------ Shared for garbage collection reasons ----------------- */
        inline static std::vector<VariableScopeMap> VariablesCallStack;
        
        /* ------------------ Rendor Memory(for tri-color marking) ------------------ */
        inline static std::unordered_map<std::string, std::tuple<size_t, GCColor>, string_hash, std::equal_to<>> Objects;
        inline static std::vector<TypeObject> WhiteObjects; // objects that need to be yeeted
        inline static std::vector<TypeObject> GreyObjects;  // objects that need to be scanned
        inline static std::vector<TypeObject> BlackObjects; // objects that have been scanned
        inline static std::stack<std::tuple<Type*, TypeObject_U>, std::list<std::tuple<Type*, TypeObject_U>>> RendorStack;

        inline static const std::unordered_map<GCColor, std::vector<TypeObject>*> VectorMapping
        {
            {GCColor::BLACK, &BlackObjects},
            {GCColor::GREY,  &GreyObjects},
            {GCColor::WHITE, &WhiteObjects},
        };
        /* -------------- Pointers to certain parts of Rendor's memory -------------- */
        inline static VariableScopeMap *CurrentScopeVariables = NULL;
        inline static VariableScopeMap *GlobalVariables = NULL;

        /* ----------------------- if statement related things ---------------------- */
        inline static std::unordered_map<std::string, RendorFunctionPtr, string_hash, std::equal_to<>> CppFunctions;
        

        /* ------------------------------ Bytecode Loop ----------------------------- */
        static void ByteCodeLoop(std::vector<std::string_view>& ByteCode);

        /* ----------------------------- Definition Loop ---------------------------- */
        static void ByteCodeLoopDefinition(const boost::interprocess::mapped_region& Code);

        /* ---------------------------- Garbage Collector --------------------------- */
        static void GarbageCollector();
        static void MarkConstantBlack(TypeObject Const);
        static void CreateConstant(std::string_view Constant);
        static void GetConstFromVariable(const std::string& Variable);
        static void FindConst(std::string_view Const);

        /* -------------------------------- Math Eval ------------------------------- */
        static void PostFixEval(std::string_view PostFixOperation);
        static void MathVariantToConst(MathVariant& MathConst);
};

#endif // INTERPRETER_HPP

#ifndef INTERPRETER
#define INTERPRETER

#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <functional>
#include <optional>
#include <memory>
#include <variant>
#include <tuple>

using std::cout;
using std::cin;
using std::string;
using std::string_view;
using std::find_if;
using std::find;
using std::remove_if;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::tuple;
using std::array;
using std::variant;
using std::ifstream;
using std::pair;
using std::map;
using std::unordered_map;
using std::equal_to;
using std::function;
using std::optional;
using std::list;
using std::vector;

// Other Parts of the Rendor Engine Interpreter 
#include "RendorInterpreter/Resources/RendorTypes.hpp"
#include "RendorInterpreter/Resources/VariableType.hpp"
#include "RendorInterpreter/Resources/RendorDefinitions.hpp"
#include "RendorInterpreter/Resources/CodeObjects.hpp"
#include "RendorInterpreter/Resources/ByteCodeEnum.hpp"
#include "RendorInterpreter/Resources/RendorGCData.hpp"
#include "RendorInterpreter/JIT/RendorJit.hpp"
#include "Exceptions.hpp"
#include "UnorderedMapLookUp.hpp"

using rengc::RendorGCData;
using rengc::RendorStack;

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/tokenizer.hpp>

using boost::interprocess::mapped_region;


class Interpreter
{
    public:
        /* -------------------- Used for internals in other files ------------------- */
        static void ExecuteByteCode(const boost::interprocess::mapped_region& File);
        static void DisposeConst(TypeObject RendorConstID);

        /* ----------- Used to grab function arguments for JITed functions ---------- */
        static int64_t GrabInt64FromArgs();
        static double GrabDoubleFromArgs();
        static const char* GrabStringFromArgs();
        static bool GrabBoolFromArgs();

        /* --------------- Used to grab constants for JITed functions --------------- */
        static int64_t GrabInt64FromConsts();
        static double GrabDoubleFromConsts();
        static const char* GrabStringFromConsts();
        static bool GrabBoolFromConsts();

        /* ------------- Grab the type of the currently pointed constant ------------ */
        static VariableType GrabPointedType();
    private:
        /* -------------------------------------------------------------------------- */
        /*                             Built in functions                             */
        /* -------------------------------------------------------------------------- */
        static std::optional<TypeObject> RENDOR_ECHO_FUNCTION  (std::vector<TypeObjectPtr>& EchoArgs);
        static std::optional<TypeObject> RENDOR_INPUT_FUNCTION (std::vector<TypeObjectPtr>& InputArgs);
        static std::optional<TypeObject> RENDOR_SUM_FUNCTION   (std::vector<TypeObjectPtr>& SumArgs);

        /* -------------------------------------------------------------------------- */
        /*                            Interpreter Internals                           */
        /* -------------------------------------------------------------------------- */

        /* -------------------------------- Bytecode -------------------------------- */
        inline static const std::unordered_map<std::string, ByteCodeEnum, string_hash, std::equal_to<>> ByteCodeMapping
        {
            {"LOAD",            ByteCodeEnum::LOAD},
            {"CONST",           ByteCodeEnum::CONST_OP},
            {"ASSIGN",          ByteCodeEnum::ASSIGN},
            {"ARGUMENT",        ByteCodeEnum::ARGUMENT},
            {"DEFINE",          ByteCodeEnum::DEFINE},
            {"CALL",            ByteCodeEnum::CALL},
            {"FINALIZE_CALL",   ByteCodeEnum::FINALIZE_CALL},
            {"FUNCTION",        ByteCodeEnum::FUNCTION},
            {"OPERATOR",        ByteCodeEnum::OPERATOR},
            {"JMP_IF_FALSE",    ByteCodeEnum::JMP_IF_FALSE},
            {"ENDIF",           ByteCodeEnum::ENDIF},
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
        /* ----------------------------- Rendor's Memory ---------------------------- */
        inline static std::unordered_map<std::string, std::unique_ptr<Function>, string_hash, std::equal_to<>> UserDefinedFunctions; // Stores index of user defined functions in the main file
        inline static std::array<TypeObjectPtr, 2> Constants; // for temp access to constants
        inline static size_t ConstantIndex = 0;

        /* ------------------ Shared for garbage collection reasons ----------------- */
        inline static std::vector<VariableScopeMap> VariablesCallStack;
        inline static std::vector<std::vector<TypeObjectPtr>> FunctionArgsCallStack;

        /* ------------------------------ jit compiling ----------------------------- */
        inline static RendorJIT JitCompiler;
        
        /* ------------------ Rendor Memory(for tri-color marking) ------------------ */
        inline static rengc::RendorGCData<TypeObject> ObjectMemory;
        inline static rengc::RendorStack<TypeObject, 50> ObjectStack;

        /* -------------- Pointers to certain parts of Rendor's memory -------------- */
        inline static VariableScopeMap *CurrentScopeVariables = NULL;
        inline static VariableScopeMap *GlobalVariables = NULL;

        /* ----------------------- if statement related things ---------------------- */
        inline static std::vector<bool> IfStatementBoolResult; 

        // Functions
        inline static std::unordered_map<std::string, StringVector, string_hash, std::equal_to<>> FunctionArgs  {{"echo",  {"EchoArgs"}}, 
                                                                                                {"input",  {"InputArgs"}}, 
                                                                                                {"sum",    {"SumArgs"}}};
                                                                    
        inline static std::unordered_map<std::string, RendorFunctionPtr, string_hash, std::equal_to<>> BuiltInFunctions {{"echo",  RENDOR_ECHO_FUNCTION}, 
                                                                                                        {"input", RENDOR_INPUT_FUNCTION}, 
                                                                                                        {"sum",   RENDOR_SUM_FUNCTION}};
                                                                

        /* ------------------------------ Bytecode Loop ----------------------------- */
        static void ByteCodeLoop(std::vector<std::string_view>& ByteCode);

        /* ----------------------------- Definition Loop ---------------------------- */
        static void ByteCodeLoopDefinition(const boost::interprocess::mapped_region& Code);

        /* ---------------------------- Garbage Collector --------------------------- */
        static void GarbageCollector();
        static void MarkConstantBlack(TypeObject Const);
        static TypeTuple ParseConstant(std::string_view Constant);
        static TypeObject CreateConstant(std::string_view Constant);
        static void AddToConstantsArray(TypeObjectPtr ConstantToBePlaced);
        static void AddToConstantsArray(TypeTuple ConstantToBePlaced);
        static TypeObjectPtr GetConstFromVariable(const std::string& Variable);

        /* -------------------------------- Math Eval ------------------------------- */
        static std::string PostFixEval(std::string_view PostFixOperation);

        enum class RendorState
        {
            None,
            FunctionCall
        };

        inline static std::vector<RendorState> RendorStateIDList {RendorState::None};
};

#endif // INTERPRETER

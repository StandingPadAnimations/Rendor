#ifndef INTERPRETER
#define INTERPRETER

#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <functional>
#include <optional>
#include <memory>

// Other Parts of the Rendor Engine Interpreter 
#include "RendorInterpreter/MathEvaluator.hpp"
#include "RendorInterpreter/RendorTypes.hpp"
#include "RendorInterpreter/VariableType.hpp"
#include "Exceptions.hpp"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/circular_buffer.hpp>



/* ---------------------------- Typedefs for STL ---------------------------- */
typedef std::shared_ptr<Type> TypeObject;
typedef std::weak_ptr<Type> TypeObjectPtr;
typedef std::vector<TypeObjectPtr> TypePtrVector;
typedef std::vector<std::string> StringVector;
typedef std::unordered_map<std::string, std::unique_ptr<Variable>> VariableScopeMap;
typedef std::function<std::optional<TypeObject>(std::vector<TypeObjectPtr>&)> RendorFunctionPtr;

class Interpreter
{
    public:
        static void ExecuteByteCode(std::ifstream& File);
        static void DisposeConst(TypeObject RendorConstID);

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
        /* ----------------------------- Rendor's Memory ---------------------------- */
        inline static std::unordered_map<std::string, size_t> UserDefinedFunctions; // Stores index of user defined functions in the main file
        inline static std::vector<TypeObjectPtr> Constants; // for temp access to constants
        inline static size_t ConstantIndex = 0;

        /* ------------------ Shared for garbage collection reasons ----------------- */
        inline static std::vector<VariableScopeMap> VariablesCallStack;
        inline static std::vector<std::vector<TypeObjectPtr>> FunctionArgsCallStack;
        
        /* ------------------ Rendor Memory(for tri-color marking) ------------------ */
        inline static std::vector<TypeObject> WhiteObjects; // objects that need to be yeeted
        inline static std::vector<TypeObject> GreyObjects;  // objects that need to be scanned
        inline static std::vector<TypeObject> BlackObjects; // objects that have been scanned
        inline static std::vector<TypeObjectPtr> Objects;   // All Objects 

        /* -------------- Pointers to certain parts of Rendor's memory -------------- */
        inline static VariableScopeMap *CurrentScopeVariables = NULL;
        inline static VariableScopeMap *GlobalVariables = NULL;

        /* ----------------------- if statement related things ---------------------- */
        inline static std::vector<bool> IfStatementBoolResult; 

        // Functions
        inline static std::map<std::string, StringVector> FunctionArgs {{"echo",  {"EchoArgs"}}, 
                                                                        {"input",  {"InputArgs"}}, 
                                                                        {"sum",    {"SumArgs"}}};
                                                                    
        inline static std::map<std::string, RendorFunctionPtr> BuiltInFunctions{{"echo",  RENDOR_ECHO_FUNCTION}, 
                                                                                {"input", RENDOR_INPUT_FUNCTION}, 
                                                                                {"sum",   RENDOR_SUM_FUNCTION}};
                                                                

        /* ------------------------------ Bytecode Loop ----------------------------- */
        static void ByteCodeLoop(std::vector<std::string>& ByteCode, size_t StartIndex);

        /* ----------------------------- Definition Loop ---------------------------- */
        static void ByteCodeLoopDefinition(std::vector<std::string>& ByteCode, size_t StartIndex);

        /* ---------------------------- Garbage Collector --------------------------- */
        static void GarbageCollector();
        static void MarkConstantBlack(TypeObject Const);
        static TypeObject CreateConstant(std::string_view Constant);
        static void AddToConstantsVector(TypeObjectPtr ConstantToBePlaced);

        enum class RendorState
        {
            None,
            FunctionCall
        };

        inline static std::vector<RendorState> RendorStateIDList {RendorState::None};
};

#endif // INTERPRETER

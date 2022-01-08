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
#include "RendorInterpreter/Unique_ptr_ref.hpp"
#include "RendorInterpreter/CompareRendorIDs.hpp"
#include "Exceptions.hpp"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/circular_buffer.hpp>


/* ---------------------- Typedefs for Rendor Templates --------------------- */
typedef ren::unique_ptr_ref<Type> TypeObjectPtr;

/* ---------------------------- Typedefs for STL ---------------------------- */
typedef std::unique_ptr<Type> TypeObject;
typedef std::vector<TypeObjectPtr> TypePtrVector;
typedef std::vector<std::string> StringVector;
typedef std::map<std::string, std::unique_ptr<Variable>> VariableScopeMap;
typedef std::function<std::optional<TypeObjectPtr>(std::vector<TypeObjectPtr>&)> RendorFunctionPtr;

class Interpreter
{
    public:
        static void ExecuteByteCode(std::ifstream& File);

    private:
        /* -------------------------------------------------------------------------- */
        /*                             Built in functions                             */
        /* -------------------------------------------------------------------------- */
        static std::optional<TypeObjectPtr> RENDOR_ECHO_FUNCTION  (std::vector<TypeObjectPtr>& EchoArgs);
        static std::optional<TypeObjectPtr> RENDOR_INPUT_FUNCTION (std::vector<TypeObjectPtr>& InputArgs);
        static std::optional<TypeObjectPtr> RENDOR_SUM_FUNCTION   (std::vector<TypeObjectPtr>& SumArgs);

        /* -------------------------------------------------------------------------- */
        /*                            Interpreter Internals                           */
        /* -------------------------------------------------------------------------- */
        /* ----------------------------- Rendor's Memory ---------------------------- */
        inline static std::map<std::string, size_t> UserDefinedFunctions; // Stores index of user defined functions in the main file
        inline static boost::circular_buffer_space_optimized<TypeObjectPtr> Constants{2}; // for temp access to constants
        inline static size_t ConstantIndex = 0;

        /* ------------------ Shared for garbage collection reasons ----------------- */
        inline static std::vector<std::string_view> DisposedVariables; // All variables that need to be destroyed are placed here
        inline static std::vector<VariableScopeMap> VariablesCallStack;
        inline static std::list<std::vector<TypeObjectPtr>> FunctionArgsCallStack;
        
        /* ------------------ Rendor Memory(for tri-color marking) ------------------ */
        inline static std::vector<TypeObject> WhiteObjects; // All objects that will get deleted
        inline static std::vector<TypeObject> GreyObjects; // All objects that won't be deleted, but still need to be scanned
        inline static std::vector<TypeObject> BlackObjects; // Al objects that have been fully scanned

        inline static std::vector<TypeObjectPtr> Objects; // All objects 

        inline static std::map<std::string_view, std::vector<std::string_view>> SharedVariables;

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
        static TypeObjectPtr CreateConstant(std::string_view Constant);
        static void MarkConstantBlack(TypeObjectPtr& Const);

        enum class RendorState
        {
            None,
            FunctionCall
        };

        static RendorState RendorStateID;
};

#endif // INTERPRETER

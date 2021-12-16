#ifndef INTERPRETER
#define INTERPRETER

#include <iostream>
#include <string>
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
#include "Exceptions.hpp"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/circular_buffer.hpp>

/* -------------------------------- Typedefs -------------------------------- */
typedef std::shared_ptr<Type> TypeObject;
typedef std::map<std::string, std::unique_ptr<Variable>> VariableScopeMap;
typedef std::vector<std::string> StringVector;
typedef std::function<std::optional<TypeObject>(std::vector<TypeObject>&)> RendorFunctionPtr;
class Interpreter
{
    public:
        static void ExecuteByteCode(std::ifstream& File);

    private:
        /* --------------------------- Built in functions --------------------------- */
        static std::optional<TypeObject> RENDOR_ECHO_FUNCTION  (std::vector<TypeObject>& EchoArgs);
        static std::optional<TypeObject> RENDOR_INPUT_FUNCTION (std::vector<TypeObject>& InputArgs);
        static std::optional<TypeObject> RENDOR_SUM_FUNCTION   (std::vector<TypeObject>& SumArgs);


        /* ----------------------------- Rendor's Memory ---------------------------- */
        inline static std::map<std::string, size_t> UserDefinedFunctions; // Stores index of user defined functions in the main file
        inline static boost::circular_buffer_space_optimized<TypeObject> Constants{2}; // for temp access to constants
        inline static size_t ConstantIndex = 0;

        /* ------------------ Shared for garbage collection reasons ----------------- */
        inline static std::vector<std::string> DisposedVariables; // All variables that need to be destroyed are placed here; This does not apply to functions
        inline static std::vector<VariableScopeMap> VariablesCallStack;
        inline static std::list<TypeObject> PermenentMemory; // is a list for efficency reasons
        inline static std::list<std::vector<TypeObject>> FunctionArgsCallStack;

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
        static void GarbageCollector(bool RemoveConsts);
        static TypeObject CreateConstant(std::string_view Constant);
};





#endif // INTERPRETER

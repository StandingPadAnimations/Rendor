#ifndef RENDOR_DEF_HPP
#define RENDOR_DEF_HPP
#include <memory>
#include <vector>
#include <functional>
#include <variant>
#include <optional>
#include <cstdint>
#include <tuple>

using std::unique_ptr;
using std::tuple;
using std::variant;
using std::function;
using std::optional;
using std::vector;
using std::equal_to;

#include "RendorInterpreter/Resources/RendorTypes.hpp"
#include "RendorInterpreter/Resources/VariableType.hpp"
#include "UnorderedMapLookUp.hpp"
/* ---------------------------- Typedefs for STL ---------------------------- */
using TypeObject = std::shared_ptr<Type>;
using TypeObjectPtr = std::weak_ptr<Type>;
using TypePtrVector = std::vector<TypeObjectPtr>;
using StringVector = std::vector<std::string>;
using VariableScopeMap = std::unordered_map<std::string, std::unique_ptr<Variable>, string_hash, std::equal_to<>>;
using RendorFunctionPtr = std::function<std::optional<TypeObject>(std::vector<TypeObjectPtr>&)>;
using MathVariant = std::variant<int64_t, double>;
using TypeTuple = std::tuple<TypeObjectPtr, TypeObjectPtr>;

/* ----------------------------- Typedef for JIT ---------------------------- */
typedef void (*RendorFunc)(void);


#endif // RENDOR_DEF_HPP
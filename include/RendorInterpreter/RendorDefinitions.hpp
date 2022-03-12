#ifndef RENDOR_DEF_HPP
#define RENDOR_DEF_HPP
#include <memory>
#include <vector>
#include <functional>
#include <variant>
#include <optional>
#include <cstdint>

#include "RendorInterpreter/RendorTypes.hpp"
#include "RendorInterpreter/VariableType.hpp"
#include "UnorderedMapLookUp.hpp"
/* ---------------------------- Typedefs for STL ---------------------------- */
typedef std::shared_ptr<Type> TypeObject;
typedef std::weak_ptr<Type> TypeObjectPtr;
typedef std::unique_ptr<Type> TypeObject_U;
typedef std::vector<TypeObjectPtr> TypePtrVector;
typedef std::vector<std::string> StringVector;
typedef std::unordered_map<std::string, std::unique_ptr<Variable>, string_hash, std::equal_to<>> VariableScopeMap;
typedef std::function<void()> RendorFunctionPtr;
typedef std::variant<int64_t, double> MathVariant;

#endif // RENDOR_DEF_HPP
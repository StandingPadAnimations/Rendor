#ifndef RENDOR_EXCEPTIONS_HPP
#define RENDOR_EXCEPTIONS_HPP
#include <exception>
#include <iostream>
#include <string>
#include <string_view>

#include "RendorEngine.hpp"

#include <fmt/color.h>

namespace error{
    // Exception for Rendor in general
    class RendorException: public std::exception {
        private:
            std::string message_;
        public:
            explicit RendorException(const std::string& message);
            const char* what() const noexcept override {
                return message_.c_str();
            }
    };

    inline RendorException::RendorException(const std::string& message) : message_(message) {}

    // Compiler exceptions 
    class CompilerRendorException: public std::exception {
        private:
            std::string message_;
        public:
            explicit CompilerRendorException(const std::string& message, const size_t LineNumber);
            const char* what() const noexcept override {
                return message_.c_str();
            }
    };

    inline CompilerRendorException::CompilerRendorException(const std::string& message, const size_t LineNumber) : message_(message) 
    {
        size_t CurrentLine = 1;
        for (const auto& Line : RendorMapping::crange(RendorEngineCompiler::RendorFileMemory))
        {
            if (CurrentLine < LineNumber)
            {
                ++CurrentLine;
            }
            else
            {
                message_ = fmt::format("{}:\n{}", message, Line);
                break;
            }
        }
    }

    inline void LogWarning(std::string_view Warning)
    {
        if (RendorEngineCompiler::WarningsToErrors)
        {
            throw error::RendorException(std::string{Warning});
        }
        fmt::print(fmt::fg(fmt::color::purple), "{}\n", Warning);
    }
}

#endif // RENDOR_EXCEPTIONS_HPP

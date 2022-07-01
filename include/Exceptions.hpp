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
    using RendorException = std::runtime_error;

    // Compiler exceptions 
    class CompilerRendorException: public std::exception 
    {
        private:
            std::string m_message;
        public:
            explicit CompilerRendorException(const std::string& message, const size_t LineNumber)
            {
                size_t CurrentLine = 1;
                for (const auto& Line : RendorMapping::crange(RendorEngineCompiler::RendorFileMemory))
                {
                    if (CurrentLine == LineNumber)
                    {
                        m_message = fmt::format("{}:\n{}", message, Line);
                    }
                    else
                    {
                        ++CurrentLine;
                    }
                }
            }
            const char* what() const noexcept override 
            {
                return m_message.c_str();
            }
    };

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

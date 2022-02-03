#ifndef RENDORERROR
#define RENDORERROR
#include <exception>
#include <iostream>
#include <string>
#include <string_view>

#include "cpp-terminal/base.hpp"

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

    inline RendorException::RendorException(const std::string& message) : message_(message) {
    }

    inline void LogWarning(std::string_view Warning)
    {
        std::cout << Term::color(Term::fg::bright_magenta) << Warning << std::endl;
    }
}

#endif // RENDORERROR

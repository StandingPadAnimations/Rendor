#pragma once

// Imports needed. All classes here use the exception class.
#include <exception>
#include <iostream>
#include <string>

// All classes work by throwing them as exceptions when a error occurs, then catching it in the main function within Rendor.cpp
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

    // Exception for the Rendor Parser
    class ParserException: public std::exception {
        private:
            std::string message_;
        public:
            explicit ParserException(const std::string& message);
            const char* what() const noexcept override {
                return message_.c_str();
            }
    };
    inline ParserException::ParserException(const std::string& message) : message_(message) {
    }

    // Exception for the Rendor Evaluator
    class EvalException: public std::exception {
        private:
            std::string message_;
        public:
            explicit EvalException(const std::string& message);
            const char* what() const noexcept override {
                return message_.c_str();
            }
    };
    inline EvalException::EvalException(const std::string& message) : message_(message) {
    }
}

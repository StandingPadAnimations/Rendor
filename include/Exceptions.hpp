#pragma once

// Imports needed. All classes here use the exception class.
#include <exception>
#include <iostream>
#include <string>

// All classes work by throwing them as exceptions when a error occurs, then catching it in the main function within Rendor.cpp
namespace error{
    // Exception for Rendor in general as well as the Lexer as the Lexer should have few errors
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
}

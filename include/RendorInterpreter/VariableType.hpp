#ifndef RENDOR_VARIABLE_TYPE_HPP
#define RENDOR_VARIABLE_TYPE_HPP
#include <string>
#include <string_view>
#include <vector> 
#include <memory>

#include "RendorInterpreter/RendorTypes.hpp"


struct Variable
{
    std::string m_Name;
    std::string m_ID;
    std::shared_ptr<Type> m_ValueClass;

    explicit Variable(std::string Name);

    // During variable destruction 
    virtual ~Variable();

    private:
        inline static uint64_t IntForID = 0;
};

#endif // RENDOR_VARIABLE_TYPE_HPP
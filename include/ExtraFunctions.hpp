#ifndef EXTRAFUNCTIONS
#define EXTRAFUNCTIONS

/*
This file is just to make my life easier
*/


#include <iostream>
#include <string>
#include <vector>
#include <sstream>



namespace ex{
class Extra{
    public:
        // Creates the variables and functions we'll use all the time for the Rendor programming language
        std::vector<std::string> Split(std::string, char);
        std::vector<std::string> MultiSplit(std::string, std::string);
        std::string RemoveSpace(std::string);
        std::string trim(const std::string&, const std::string&);
        std::string reduce(const std::string&, const std::string&, const std::string&);
};
}

#endif // EXTRAFUNCTIONS

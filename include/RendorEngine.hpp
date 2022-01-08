#ifndef RENDOR_ENGINE_HPP
#define RENDOR_ENGINE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Boost libraries and macros
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace bfs = boost::filesystem;

#define RENDOR_VERSION 0
#define RENDOR_BINARY_NAME "rendorc.exe"

struct RendorEngineInterpreter{
    static void run(const std::string);
};

struct RendorEngineCompiler{
    static void run(const std::string& FileInput, std::vector<std::string>& Arguments);
};

#endif // RENDOR_ENGINE_HPP
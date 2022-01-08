#ifndef RENDORENGINE
#define RENDORENGINE

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "cpp-terminal/base.hpp"

// Boost libraries and macros
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace bfs = boost::filesystem;

struct RendorEngineInterpreter{
    static void run(const std::string);
};

struct RendorEngineCompiler{
    static void run(const std::string, char *argv[]);
};

#endif // RENDORENGINE
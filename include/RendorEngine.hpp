#ifndef RENDOR_ENGINE_HPP
#define RENDOR_ENGINE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::vector;
using std::fstream;
using std::string;

#include "RendorBoostFileMapper.hpp"

// Boost libraries and macros
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/trim.hpp>
using boost::format;
namespace bfs = boost::filesystem;

#define RENDOR_VERSION 0

struct RendorEngineInterpreter
{
    static void run(const std::string);
};

struct RendorEngineCompiler
{
    inline static bool WarningsToErrors = false;
    inline static bool DebugMode = false;
    inline static bool OptimizeByteCode = true;
    static void run(const std::string& FileInput, std::vector<std::string>& Arguments);
};

#endif // RENDOR_ENGINE_HPP
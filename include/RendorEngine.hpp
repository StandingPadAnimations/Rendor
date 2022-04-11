#ifndef RENDOR_ENGINE_HPP
#define RENDOR_ENGINE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "RendorBoostFileMapper.hpp"

#define FMT_HEADER_ONLY 1
#include <fmt/format.h>
#include <fmt/color.h>

#include <boost/algorithm/string/trim.hpp>

namespace fs = std::filesystem;

#define RENDOR_VERSION 0

struct RendorEngineInterpreter
{
    /* ----------------------------------- Run ---------------------------------- */
    static void run(const std::string);
};

struct RendorEngineCompiler
{
    /* ---------------------------------- Flags --------------------------------- */
    inline static bool WarningsToErrors = false;
    inline static bool DebugMode = false;
    inline static bool OptimizeByteCode = true;
    inline static std::vector<std::string> ByteCode;

    /* ----------------------------------- Run ---------------------------------- */
    static void run(const std::string& FileInput, std::vector<std::string>& Arguments);
};

#endif // RENDOR_ENGINE_HPP
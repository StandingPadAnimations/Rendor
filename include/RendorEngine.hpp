#ifndef RENDOR_ENGINE_HPP
#define RENDOR_ENGINE_HPP

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "RendorCompiler/BackEnd/ByteCodeContext.hpp"
#include "RendorBoostFileMapper.hpp"
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
    inline static RendorContext EngineContext;
    inline static bool WarningsToErrors = false;
    inline static bool DebugMode = false;
    inline static bool OptimizeByteCode = true;
    inline static std::vector<std::string> ByteCode;
    inline static std::string OutputPath = "";

    inline static boost::interprocess::file_mapping File;
    inline static boost::interprocess::mapped_region RendorFileMemory;

    /* ----------------------------------- Run ---------------------------------- */
    static void run(const std::string& FileInput, std::vector<std::string_view>& Arguments);

    RendorEngineCompiler()
    {
        ByteCode.reserve(1000);
    }
};

#endif // RENDOR_ENGINE_HPP
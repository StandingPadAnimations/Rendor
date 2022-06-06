#ifndef RENDOR_ENGINE_HPP
#define RENDOR_ENGINE_HPP

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "RendorBoostFileMapper.hpp"
#include "RendorAPI/FunctionTable.hpp"

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
    inline static FunctionTable Context;
    inline static bool WarningsToErrors = false;
    inline static bool DebugMode = false;
    inline static bool OptimizeByteCode = true;
    inline static std::vector<std::string> ByteCode;
    inline static std::string OutputPath = "";

    inline static boost::interprocess::file_mapping File;
    inline static boost::interprocess::mapped_region RendorFileMemory;

    inline constexpr static uint32_t MajorVersion = 1;
    inline constexpr static uint32_t MinorVersion = 2;
    inline static uint32_t MethodCount = 0;

    /* ----------------------------------- Run ---------------------------------- */
    static void run(const std::string& FileInput, std::vector<std::string_view>& Arguments);

    RendorEngineCompiler()
    {
        ByteCode.reserve(1000);
    }
};

#endif // RENDOR_ENGINE_HPP
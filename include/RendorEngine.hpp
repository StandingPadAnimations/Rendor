#ifndef RENDOR_ENGINE_HPP
#define RENDOR_ENGINE_HPP

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "RendorCompiler/BackEnd/IR/IRContext.hpp"
#include "RendorAPI/FunctionTable.hpp"
#include "RendorBoostFileMapper.hpp"

#define FMT_HEADER_ONLY 1
#include <fmt/format.h>
#include <fmt/color.h>

#include <boost/algorithm/string/trim.hpp>

#define RENDOR_VERSION 0

struct RendorEngineInterpreter
{
    /* ----------------------------------- Run ---------------------------------- */
    static void run(const std::string) noexcept;
};

struct RendorEngineCompiler
{
    /* ---------------------------------- Flags --------------------------------- */
    inline static RendorIR::IR_Context Context_IR;
    inline static bool WarningsToErrors = false;
    inline static bool DebugMode = false;
    inline static bool OptimizeByteCode = true;
    inline static std::vector<std::string> ByteCode;

    inline static boost::interprocess::file_mapping File;
    inline static boost::interprocess::mapped_region RendorFileMemory;

    inline constexpr static uint32_t MajorVersion = 1;
    inline constexpr static uint32_t MinorVersion = 2;
    inline static uint32_t MethodCount = 0;

    /* ----------------------------------- Run ---------------------------------- */
    static void run(const std::string& FileInput, const std::vector<std::string_view>& Arguments);

    RendorEngineCompiler()
    {
        ByteCode.reserve(1000);
    }
};

struct RendorEngineAssembler 
{
    inline static boost::interprocess::file_mapping File;
    inline static boost::interprocess::mapped_region RendorFileMemory;
    inline static bool DebugMode = false;
    static void run(const std::string& FileInput, const std::vector<std::string_view>& Arguments);
};

#endif // RENDOR_ENGINE_HPP
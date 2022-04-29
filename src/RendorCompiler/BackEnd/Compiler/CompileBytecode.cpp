#include "RendorCompiler/BackEnd/ByteCodeGenerator.hpp"
#include <bit>
#include <boost/lexical_cast.hpp>
#include <cstdint>

void RendorByteCodeGenerator::CompileByteCode(const std::string& FilePath)
{
    CrenOutput = std::ofstream(FilePath, std::ios::out | std::ios::binary);
    CompileHeader();

    for (const auto& ByteOp : *ByteCode)
    {
        std::size_t ByteCodeSpaceIndex  = ByteOp.find_first_of(' ');
        std::string_view Command  (ByteOp.begin(), ByteOp.begin() + ByteCodeSpaceIndex); // NOLINT
        std::string_view Args     (ByteOp.begin() + (ByteCodeSpaceIndex + 1), ByteOp.end()); // NOLINT
        switch (ByteCodeMapping.at(Command))
        {
            case ByteCodeEnum::CONST_OP:
            {
                std::size_t CommaIndex = Args.find_first_of(',');
                std::string Type  (Args.begin(), Args.begin() + CommaIndex);
                std::string Const (Args.begin() + (CommaIndex + 2), Args.end());
                switch (TypeMapping.at(Type))
                {
                    case ConstType::INT64:
                    {
                        int64_t ConvertedValue = boost::lexical_cast<int64_t>(Const);
                        CompileInt64(boost::endian::little_int64_buf_t{ConvertedValue});
                        break;
                    }
                    case ConstType::DOUBLE:
                    {
                        double ConvertedValue = std::stod(Const);
                        CompileDouble(boost::endian::little_float64_buf_t{ConvertedValue});
                        break;
                    }
                    case ConstType::STRING:
                    {
                        CompileString(Const);
                        break;
                    }
                    case ConstType::BOOL:
                    {
                        int32_t ConvertedValue = boost::lexical_cast<int32_t>(Const);
                        CompileBool(boost::endian::little_int32_buf_at{ConvertedValue});
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                break;
            }
            case ByteCodeEnum::DEFINE:
            {
                std::size_t SpaceIndex = Args.find_first_of(' ');
                std::string ABI  (Args.begin(), Args.begin() + SpaceIndex);
                std::string NAME (Args.begin() + (SpaceIndex + 1), Args.end());
                if (ABI == ".F")
                {
                    CompileFunc(NAME);
                }
                break;
            }
            case ByteCodeEnum::ASSIGN:
            {
                CompileVar(std::string{Args});
                break;
            }
            default:
            {
                break;
            }
        }
    }
}



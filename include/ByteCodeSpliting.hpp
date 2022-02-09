#ifndef RENDOR_BYTECODE_SPLITING_HPP
#define RENDOR_BYTECODE_SPLITING_HPP
#include <string_view>
#include <array>

typedef std::array<std::string_view, 2> ByteCodeArray;
inline ByteCodeArray SplitByteCode(std::string_view ByteCodeOp)
{
    size_t ByteCodeSpaceIndex = ByteCodeOp.find_first_of(" ");
    std::string_view Command  (ByteCodeOp.begin(), ByteCodeOp.begin() + ByteCodeSpaceIndex);
    std::string_view Args     (ByteCodeOp.begin() + (ByteCodeSpaceIndex + 1), ByteCodeOp.end());
    return ByteCodeArray{Command, Args};
}

#endif // RENDOR_BYTECODE_SPLITING_HPP
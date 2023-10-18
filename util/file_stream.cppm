// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.file_stream;

import util.stream;
import std.core;

export namespace util {

enum class OpenMode : uint8_t
{
    none = 0u,
    read = 1u << 0u,
    write = 1u << 1u,
    append = 1u << 2u,
    binary = 1u << 3u
};

constexpr OpenMode operator|(OpenMode left, OpenMode right)
{
    return OpenMode(uint8_t(left) | uint8_t(right));
}

constexpr OpenMode operator&(OpenMode left, OpenMode right)
{
    return OpenMode(uint8_t(left) & uint8_t(right));
}

constexpr OpenMode operator~(OpenMode operand)
{
    return OpenMode(~uint8_t(operand));
}

class FileStream : public Stream
{
public:
    FileStream(int handle);
    FileStream(const std::string& filePath_, OpenMode openMode);
    ~FileStream() override;
    int ReadByte() override;
    int64_t Read(uint8_t* buf, int64_t count) override;
    void Write(uint8_t x) override;
    void Write(uint8_t* buf, int64_t count) override;
    void Flush() override;
    void Seek(int64_t pos, Origin origin) override;
    int64_t Tell() override;
    int64_t Size() const;
private:
    std::string filePath;
    FILE* file;
    bool needToClose;
};

std::string ReadFile(const std::string& filePath);

} // namespace util

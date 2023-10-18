// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.file;

import wing_package.node;
import dom;
import util;
import std.core;

export namespace wing_package {

enum class FileFlags : uint8_t
{
    none = 0, exists = 1 << 0
};

inline FileFlags operator|(FileFlags left, FileFlags right)
{
    return FileFlags(uint8_t(left) | uint8_t(right));
}

inline FileFlags operator&(FileFlags left, FileFlags right)
{
    return FileFlags(uint8_t(left) & uint8_t(right));
}

inline FileFlags operator~(FileFlags flags)
{
    return FileFlags(~uint8_t(flags));
}

class File : public Node
{
public:
    File();
    File(const std::string& name_);
    File(NodeKind nodeKind_, const std::string& name_);
    uintmax_t Size() const { return size; }
    void SetSize(uintmax_t size_) { size = size_; }
    const std::chrono::time_point<std::chrono::file_clock>& Time() const { return time; }
    void SetTime(const std::chrono::time_point<std::chrono::file_clock>& time_) { time = time_; }
    //time_t Time() const { return time; }
    //void SetTime(const time_t time_) { time = time_; }
    const std::string& Hash() const { return hash; }
    void SetHash(const std::string& hash_) { hash = hash_; }
    std::string ComputeHash() const;
    FileFlags Flags() const { return flags; }
    void SetFlags(FileFlags flags_) { flags = flags_; }
    void SetFlag(FileFlags flag, bool value);
    bool GetFlag(FileFlags flag) const { return (flags & flag) != FileFlags::none; }
    bool Changed() const;
    void WriteIndex(util::BinaryStreamWriter& writer) override;
    void ReadIndex(util::BinaryStreamReader& reader) override;
    void WriteData(util::BinaryStreamWriter& writer) override;
    void ReadData(util::BinaryStreamReader& reader) override;
    void Remove();
    void Uninstall() override;
    xml::Element* ToXml() const override;
private:
    uintmax_t size;
    std::chrono::time_point<std::chrono::file_clock> time;
    std::string hash;
    FileFlags flags;
};

} // namespace wing_package

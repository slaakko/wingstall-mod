// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.directory;

import wing_package.node;
import dom;
import util;
import std.core;

export namespace wing_package {

class File;
class PathMatcher;

enum class DirectoryFlags : uint8_t
{
    none = 0, exists = 1 << 0
};

inline DirectoryFlags operator|(DirectoryFlags left, DirectoryFlags right)
{
    return DirectoryFlags(uint8_t(left) | uint8_t(right));
}

inline DirectoryFlags operator&(DirectoryFlags left, DirectoryFlags right)
{
    return DirectoryFlags(uint8_t(left) & uint8_t(right));
}

inline DirectoryFlags operator~(DirectoryFlags flags)
{
    return DirectoryFlags(~uint8_t(flags));
}

class Directory : public Node
{
public:
    Directory();
    Directory(const std::string& name_);
    Directory(PathMatcher& pathMatcher, const std::string& name, const std::chrono::time_point<std::chrono::file_clock>& time_, xml::Element* element);
    int Level() const;
    const std::chrono::time_point<std::chrono::file_clock>& Time() const { return time; }
    void SetTime(const std::chrono::time_point<std::chrono::file_clock>& time_) { time = time_; }
    DirectoryFlags Flags() const { return flags; }
    void SetFlags(DirectoryFlags flags_) { flags = flags_; }
    void SetFlag(DirectoryFlags flag, bool value);
    bool GetFlag(DirectoryFlags flag) const { return (flags & flag) != DirectoryFlags::none; }
    const std::vector<std::unique_ptr<Directory>>& Directories() const { return directories; }
    void AddDirectory(Directory* directory);
    const std::vector<std::unique_ptr<File>>& Files() const { return files; }
    void AddFile(File* file);
    void WriteIndex(util::BinaryStreamWriter& writer) override;
    void ReadIndex(util::BinaryStreamReader& reader) override;
    void WriteData(util::BinaryStreamWriter& writer) override;
    void ReadData(util::BinaryStreamReader& reader) override;
    bool HasDirectoriesOrFiles();
    void Remove();
    void Uninstall() override;
    xml::Element* ToXml() const override;
private:
    std::chrono::time_point<std::chrono::file_clock> time;
    DirectoryFlags flags;
    std::vector<std::unique_ptr<Directory>> directories;
    std::vector<std::unique_ptr<File>> files;
};

} // namespace wing_package

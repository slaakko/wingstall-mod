// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.component;

import wing_package.node;
import wing_package.directory;
import dom;
import util;
import std.core;

export namespace wing_package {

class File;
class PathMatcher;

enum class Compression : uint8_t
{
    none, deflate//, bzip2
};

std::string CompressionStr(Compression compression);
Compression ParseCompressionStr(const std::string& compressionStr);

class Component : public Node
{
public:
    Component();
    Component(const std::string& name_);
    Component(NodeKind nodeKind_, const std::string& name_);
    Component(PathMatcher& pathMatcher, xml::Element* element);
    virtual void RunCommands();
    virtual void CreateInstallationInfo();
    virtual void RemoveInstallationInfo();
    const std::vector<std::unique_ptr<Directory>>& Directories() const { return directories; }
    void AddDirectory(Directory* directory);
    void AddFile(File* file);
    virtual void Write(util::Streams& streams);
    virtual void Read(util::Streams& streams);
    void WriteIndex(util::BinaryStreamWriter& writer) override;
    void ReadIndex(util::BinaryStreamReader& reader) override;
    void WriteData(util::BinaryStreamWriter& writer) override;
    void ReadData(util::BinaryStreamReader& reader) override;
    void Uninstall() override;
    xml::Element* ToXml() const override;
private:
    std::vector<std::unique_ptr<Directory>> directories;
    std::vector<std::unique_ptr<File>> files;
};

} // namespace wing_package

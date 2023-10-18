// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.preinstall_component;

import wing_package.directory;
import wing_package.file;
import wing_package.component;
import wing_package.path_matcher;
import dom;
import util;
import std.core;

export namespace wing_package {

class PreinstallComponent : public Component
{
public:
    PreinstallComponent();
    PreinstallComponent(PathMatcher& pathMatcher, xml::Element* element);
    void Write(util::Streams& streams) override;
    void Read(util::Streams& streams) override;
    void RunCommands() override;
    void AddFile(File* file);
    void WriteIndex(util::BinaryStreamWriter& writer) override;
    void ReadIndex(util::BinaryStreamReader& reader) override;
    void WriteData(util::BinaryStreamWriter& writer) override;
    void ReadData(util::BinaryStreamReader& reader) override;
private:
    std::vector<FileInfo> fileInfos;
    std::vector<std::unique_ptr<File>> files;
    std::vector<std::string> commands;
    void RemovePreinstallDir();
};

} // namespace wing_package

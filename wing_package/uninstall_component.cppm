// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.uninstall_component;

import wing_package.component;
import wing_package.file;
import util;
import std.core;

export namespace wing_package {

class UninstallComponent : public Component
{
public:
    UninstallComponent();
    void Initialize();
    void AddFile(File* file);
    void WriteIndex(util::BinaryStreamWriter& writer) override;
    void ReadIndex(util::BinaryStreamReader& reader) override;
    void WriteData(util::BinaryStreamWriter& writer) override;
    void ReadData(util::BinaryStreamReader& reader) override;
    void Uninstall() override;
private:
    std::vector<std::unique_ptr<File>> files;
    void ScheduleRootDirToBeRemoved();
};

} // namespace wing_package

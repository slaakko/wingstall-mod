// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.uninstall_bin_file;

import wing_package.file;
import util;
import std.core;

export namespace wing_package {

class UninstallBinFile : public File
{
public:
    UninstallBinFile();
    void WriteIndex(util::BinaryStreamWriter& writer) override;
    void ReadIndex(util::BinaryStreamReader& reader) override;
    void WriteData(util::BinaryStreamWriter& writer) override;
    void ReadData(util::BinaryStreamReader& reader) override;
    void Uninstall() override;
private:
    void Remove();
};

} // namespace wing_package

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.uninstall_bin_file;

import wing_package.package;
import wing_package.links;
import wing_package.environment;
import std.filesystem;

namespace wing_package {

UninstallBinFile::UninstallBinFile() : File(NodeKind::uninstall_bin_file, "uninstall.bin")
{
}

void UninstallBinFile::WriteIndex(util::BinaryStreamWriter& writer)
{
}

void UninstallBinFile::ReadIndex(util::BinaryStreamReader& reader)
{
}

void UninstallBinFile::WriteData(util::BinaryStreamWriter& writer)
{
}

void UninstallBinFile::ReadData(util::BinaryStreamReader& reader)
{
}

void UninstallBinFile::Uninstall()
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetFile(this);
        package->CheckInterrupted();
    }
    Node::Uninstall();
    Remove();
}

void UninstallBinFile::Remove()
{
    Package* package = GetPackage();
    if (package)
    {
        std::string filePath = util::Path::Combine(package->TargetRootDir(), Name());
        std::error_code ec;
        std::filesystem::remove(util::MakeNativeFileSystemPath(filePath), ec);
        if (ec)
        {
            package->LogError("could not remove '" + filePath + "': " + util::PlatformStringToUtf8(ec.message()));
        }
    }
}

} // namespace wing_package

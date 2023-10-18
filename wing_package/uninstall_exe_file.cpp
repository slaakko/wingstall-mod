// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.uninstall_exe_file;

import wing_package.package;
import wing_package.links;
import wing_package.environment;
import wing;
import std.filesystem;

namespace wing_package {

UninstallExeFile::UninstallExeFile() : File(NodeKind::uninstall_exe_file, "uninstall.exe")
{
}

void UninstallExeFile::SetInfo()
{
    std::string filePath = util::Path::Combine(util::Path::Combine(util::WingstallRoot(), "bin"), Name());
    SetSize(std::filesystem::file_size(util::MakeNativeFileSystemPath(filePath)));
    SetTime(std::filesystem::last_write_time(util::MakeNativeFileSystemPath(filePath)));
}

void UninstallExeFile::WriteIndex(util::BinaryStreamWriter& writer)
{
    File::WriteIndex(writer);
}

void UninstallExeFile::ReadIndex(util::BinaryStreamReader& reader)
{
    File::ReadIndex(reader);
}

void UninstallExeFile::WriteData(util::BinaryStreamWriter& writer)
{
    std::string filePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::WingstallRoot(), "bin"), Name()));
    util::FileStream fileStream(filePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream bufferedStream(fileStream);
    int64_t size = Size();
    for (int64_t i = 0; i < size; ++i)
    {
        int x = bufferedStream.ReadByte();
        if (x == -1)
        {
            throw std::runtime_error("unexpected end of file '" + filePath + "'");
        }
        else
        {
            uint8_t b = static_cast<uint8_t>(x);
            writer.Write(b);
        }
    }
    Package* package = GetPackage();
    if (package)
    {
        package->IncrementFileContentPosition(size);
    }
}

void UninstallExeFile::ReadData(util::BinaryStreamReader& reader)
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetFile(this);
        package->CheckInterrupted();
    }
    std::string filePath = util::Path::Combine(GetTargetRootDir(), Name());
    {
        util::FileStream fileStream(filePath, util::OpenMode::write | util::OpenMode::binary);
        util::BufferedStream bufferedStream(fileStream);
        int64_t n = Size();
        for (int64_t i = 0; i < n; ++i)
        {
            uint8_t x = reader.ReadByte();
            bufferedStream.Write(x);
        }
    }
    std::error_code ec;
    std::filesystem::last_write_time(util::MakeNativeFileSystemPath(filePath), Time(), ec);
    if (ec)
    {
        throw std::runtime_error("could not set write time of file '" + filePath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    if (package)
    {
        package->IncrementFileContentPosition(Size());
    }
}

void UninstallExeFile::Uninstall()
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetFile(this);
        package->CheckInterrupted();
    }
    Node::Uninstall();
    if (Rename())
    {
        ScheduleToBeRemoved();
    }
    if (package)
    {
        package->IncrementFileIndex();
    }
}

bool UninstallExeFile::Rename()
{
    Package* package = GetPackage();
    if (package)
    {
        std::string newName = util::Path::ChangeExtension(Name(), ".rmv");
        try
        {
            std::string fromPath = util::Path::Combine(package->TargetRootDir(), Name());
            std::string toPath = util::Path::Combine(package->TargetRootDir(), newName);
            wing::MoveFile(fromPath, toPath);
            SetName(newName);
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not rename file '" + Name() + "' to '" + newName + "': " + ex.what());
            return false;
        }
    }
    return true;
}

void UninstallExeFile::ScheduleToBeRemoved()
{
    Package* package = GetPackage();
    if (package)
    {
        std::string filePath = util::Path::Combine(package->TargetRootDir(), Name());
        try
        {
            wing::RemoveOnReboot(util::MakeNativePath(filePath));
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not schedule file '" + filePath + "' to be removed on reboot: " + ex.what());
        }
    }
}

} // namespace wing_package

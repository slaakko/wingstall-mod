// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.uninstall_component;

import wing_package.package;
import wing_package.uninstall_exe_file;
import wing_package.uninstall_bin_file;
import wing_package.directory;
import wing_package.links;
import wing_package.environment;
import wing;

namespace wing_package {

UninstallComponent::UninstallComponent() : Component(NodeKind::uninstall_component, "uninstall")
{
}

void UninstallComponent::Initialize()
{
    UninstallExeFile* uninstallExeFile = new UninstallExeFile();
    AddFile(uninstallExeFile);
    uninstallExeFile->SetInfo();
    UninstallBinFile* uinstallBinFile = new UninstallBinFile();
    AddFile(uinstallBinFile);
}

void UninstallComponent::AddFile(File* file)
{
    file->SetParent(this);
    files.push_back(std::unique_ptr<File>(file));
}

void UninstallComponent::WriteIndex(util::BinaryStreamWriter& writer)
{
    Component::WriteIndex(writer);
    int32_t numFiles = files.size();
    writer.Write(numFiles);
    for (int32_t i = 0; i < numFiles; ++i)
    {
        File* file = files[i].get();
        wing_package::WriteIndex(file, writer);
    }
}

void UninstallComponent::ReadIndex(util::BinaryStreamReader& reader)
{
    Component::ReadIndex(reader);
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
    }
    int32_t numFiles = reader.ReadInt();
    for (int32_t i = 0; i < numFiles; ++i)
    {
        File* file = BeginReadFile(reader);
        AddFile(file);
        file->ReadIndex(reader);
    }
}

void UninstallComponent::WriteData(util::BinaryStreamWriter& writer)
{
    for (const auto& file : files)
    {
        file->WriteData(writer);
    }
}

void UninstallComponent::ReadData(util::BinaryStreamReader& reader)
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
    }
    for (const auto& file : files)
    {
        file->ReadData(reader);
    }
}

void UninstallComponent::Uninstall()
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
    }
    for (const auto& file : files)
    {
        file->Uninstall();
    }
    ScheduleRootDirToBeRemoved();
}

void UninstallComponent::ScheduleRootDirToBeRemoved()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            wing::RemoveOnReboot(util::MakeNativePath(package->TargetRootDir()));
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not schedule directory '" + package->TargetRootDir() + "' to be removed on reboot: " + ex.what());
        }
    }
}

} // namespace wing_package

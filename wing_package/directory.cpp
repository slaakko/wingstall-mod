// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.directory;

import wing_package.package;
import wing_package.file;
import wing_package.path_matcher;
import wing_package.links;
import wing_package.environment;
import std.filesystem;

namespace wing_package {

Directory::Directory() : Node(NodeKind::directory), time(), flags(DirectoryFlags::none)
{
}

Directory::Directory(const std::string& name_) : Node(NodeKind::directory, name_), time(), flags(DirectoryFlags::none)
{
}

Directory::Directory(PathMatcher& pathMatcher, const std::string& name, const std::chrono::time_point<std::chrono::file_clock>& time_, xml::Element* element) :
    Node(NodeKind::directory), time(time_), flags(DirectoryFlags::none)
{
    if (name.empty())
    {
        std::string lineColStr;
        if (element)
        {
            lineColStr = " line " + std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col);
        }
        throw std::runtime_error("directory element has no 'name' attribute in package XML document '" + pathMatcher.XmlFilePath() + "'" + lineColStr);
    }
    SetName(name);
    pathMatcher.BeginDirectory(Name(), element);
    if (time == std::chrono::time_point<std::chrono::file_clock>())
    {
        //std::chrono::time_point<std::chrono::file_clock> fileTime = std::filesystem::last_write_time(util::MakeNativeFileSystemPath(pathMatcher.CurrentDir()));
        //const auto systemTime = std::chrono::utc_clock::to_sys(std::chrono::file_clock::to_utc(fileTime));
        //time = std::chrono::system_clock::to_time_t(systemTime);
        time = std::filesystem::last_write_time(util::MakeNativeFileSystemPath(pathMatcher.CurrentDir()));
    }
    std::vector<DirectoryInfo> directories = pathMatcher.Directories();
    for (const auto& directoryInfo : directories)
    {
        Directory* directory = new Directory(pathMatcher, directoryInfo.name, directoryInfo.time, directoryInfo.element);
        AddDirectory(directory);
    }
    std::vector<FileInfo> files = pathMatcher.Files();
    for (const auto& fileInfo : files)
    {
        File* file = new File();
        file->SetName(fileInfo.name);
        file->SetSize(fileInfo.size);
        file->SetTime(fileInfo.time);
        AddFile(file);
    }
    pathMatcher.EndDirectory();
}

void Directory::AddDirectory(Directory* directory)
{
    directory->SetParent(this);
    directories.push_back(std::unique_ptr<Directory>(directory));
}

void Directory::AddFile(File* file)
{
    file->SetParent(this);
    files.push_back(std::unique_ptr<File>(file));
}

void Directory::WriteIndex(util::BinaryStreamWriter& writer)
{
    Node::WriteIndex(writer);
    //writer.Write(time);
    writer.WriteFileTime(time);
    writer.Write(static_cast<uint8_t>(flags));
    int32_t numDirectories = directories.size();
    writer.Write(numDirectories);
    for (int32_t i = 0; i < numDirectories; ++i)
    {
        Directory* directory = directories[i].get();
        wing_package::WriteIndex(directory, writer);
    }
    int32_t numFiles = files.size();
    writer.Write(numFiles);
    for (int i = 0; i < numFiles; ++i)
    {
        File* file = files[i].get();
        wing_package::WriteIndex(file, writer);
    }
}

void Directory::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
    }
    //time = reader.ReadTime();
    time = reader.ReadFileTime();
    flags = static_cast<DirectoryFlags>(reader.ReadByte());
    int32_t numDirectories = reader.ReadInt();
    for (int32_t i = 0; i < numDirectories; ++i)
    {
        Directory* directory = BeginReadDirectory(reader);
        AddDirectory(directory);
        directory->ReadIndex(reader);
    }
    int32_t numFiles = reader.ReadInt();
    for (int32_t i = 0; i < numFiles; ++i)
    {
        File* file = BeginReadFile(reader);
        AddFile(file);
        file->ReadIndex(reader);
    }
}

int Directory::Level() const
{
    Node* parent = Parent();
    if (parent && parent->Kind() == NodeKind::directory)
    {
        return static_cast<Directory*>(parent)->Level() + 1;
    }
    else
    {
        return 0;
    }
}

void Directory::SetFlag(DirectoryFlags flag, bool value)
{
    if (value) flags = flags | flag;
    else flags = flags & ~flag;
}

void Directory::WriteData(util::BinaryStreamWriter& writer)
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
    }
    for (const auto& directory : directories)
    {
        directory->WriteData(writer);
    }
    for (const auto& file : files)
    {
        file->WriteData(writer);
    }
}

void Directory::ReadData(util::BinaryStreamReader& reader)
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
    }
    std::string directoryPath = Path(GetTargetRootDir());
    bool exists = std::filesystem::exists(util::MakeNativeFileSystemPath(directoryPath));
    SetFlag(DirectoryFlags::exists, exists);
    std::error_code ec;
    std::filesystem::create_directories(util::MakeNativeFileSystemPath(directoryPath), ec);
    if (ec)
    {
        throw std::runtime_error("could not create directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    for (const auto& directory : directories)
    {
        directory->ReadData(reader);
    }
    for (const auto& file : files)
    {
        file->ReadData(reader);
    }
    if (!exists)
    {
        //std::chrono::system_clock::time_point sysTime = std::chrono::system_clock::from_time_t(time);
        //std::chrono::file_clock::time_point fileTime = std::chrono::clock_cast<std::chrono::file_clock>(sysTime);
        //std::filesystem::last_write_time(util::MakeNativeFileSystemPath(directoryPath), fileTime, ec);
        std::filesystem::last_write_time(util::MakeNativeFileSystemPath(directoryPath), time, ec);
        if (ec)
        {
            throw std::runtime_error("could not set write time of directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
        }
    }
}

bool Directory::HasDirectoriesOrFiles()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            std::string directoryPath = Path(GetTargetRootDir());
            if (std::filesystem::exists(util::MakeNativeFileSystemPath(directoryPath)))
            {
                std::error_code ec;
                std::filesystem::directory_iterator it(directoryPath, ec);
                if (ec)
                {
                    throw std::runtime_error("could not iterate directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
                }
                while (it != std::filesystem::directory_iterator())
                {
                    if (std::filesystem::is_directory(it->status()))
                    {
                        if (it->path().filename() != "." && it->path().filename() != "..")
                        {
                            return true;
                        }
                    }
                    else if (std::filesystem::is_regular_file(it->status()))
                    {
                        return true;
                    }
                    ++it;
                }
                return false;
            }
            else
            {
                return false;
            }
        }
        catch (const std::exception& ex)
        {
            package->LogError(ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
    return false;
}

void Directory::Remove()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            std::string directoryPath = Path(GetTargetRootDir());
            std::error_code ec;
            std::filesystem::remove(util::MakeNativeFileSystemPath(directoryPath), ec);
            if (ec)
            {
                throw std::runtime_error("could not remove directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
            }
        }
        catch (const std::exception& ex)
        {
            package->LogError(ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void Directory::Uninstall()
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
    }
    Node::Uninstall();
    for (const auto& file : files)
    {
        file->Uninstall();
    }
    for (const auto& directory : directories)
    {
        directory->Uninstall();
    }
    if (!GetFlag(DirectoryFlags::exists) && !HasDirectoriesOrFiles())
    {
        Remove();
    }
}

xml::Element* Directory::ToXml() const
{
    xml::Element* element = xml::MakeElement("directory");
    element->SetAttribute("name", Name());
    //element->SetAttribute("time", util::TimeToString(time));
    element->SetAttribute("time", util::FileTimeToString(time));
    for (const auto& directory : directories)
    {
        xml::Element* child = directory->ToXml();
        element->AppendChild(child);
    }
    for (const auto& file : files)
    {
        xml::Element* child = file->ToXml();
        element->AppendChild(child);
    }
    return element;
}

} // namespace wing_package

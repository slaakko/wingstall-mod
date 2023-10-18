// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.file;

import wing_package.package;
import wing_package.links;
import wing_package.environment;
import std.filesystem;

namespace wing_package {

File::File() : Node(NodeKind::file), size(0), time(), flags(FileFlags::none)
{
}

File::File(const std::string& name_) : Node(NodeKind::file, name_), size(0), time(), flags(FileFlags::none)
{
}

File::File(NodeKind nodeKind_, const std::string& name_) : Node(nodeKind_, name_), size(0), time(), flags(FileFlags::none)
{
}

void File::WriteIndex(util::BinaryStreamWriter& writer)
{
    Node::WriteIndex(writer);
    writer.Write(static_cast<uint64_t>(size));
    //writer.WriteTime(time);
    writer.WriteFileTime(time);
    writer.Write(hash);
    writer.Write(static_cast<uint8_t>(flags));
    Package* package = GetPackage();
    if (package)
    {
        package->IncrementFileContentSize(size);
    }
}

void File::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        package->IncrementFileCount();
    }
    size = reader.ReadULong();
    //time = reader.ReadTime();
    time = reader.ReadFileTime();
    hash = reader.ReadUtf8String();
    flags = static_cast<FileFlags>(reader.ReadByte());
    if (package)
    {
        package->IncrementFileContentSize(size);
    }
}

void File::WriteData(util::BinaryStreamWriter& writer)
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
    }
    util::Sha1 sha1;
    std::string filePath = Path(GetSourceRootDir());
    util::FileStream fileStream(filePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream bufferedStream(fileStream);
    int64_t n = size;
    for (int64_t i = 0; i < n; ++i)
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
            sha1.Process(b);
        }
    }
    hash = sha1.GetDigest();
    writer.Write(hash);
    if (package)
    {
        package->IncrementFileContentPosition(size);
    }
}

void File::ReadData(util::BinaryStreamReader& reader)
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetFile(this);
        package->CheckInterrupted();
    }
    std::string filePath = Path(GetTargetRootDir());
    bool exists = std::filesystem::exists(util::MakeNativeFileSystemPath(filePath));
    SetFlag(FileFlags::exists, exists);
    {
        util::FileStream fileStream(filePath, util::OpenMode::write | util::OpenMode::binary);
        util::BufferedStream bufferedStream(fileStream);
        int64_t n = size;
        for (int64_t i = 0; i < n; ++i)
        {
            uint8_t x = reader.ReadByte();
            bufferedStream.Write(x);
        }
    }
    std::error_code ec;
    //std::chrono::system_clock::time_point sysTime = std::chrono::system_clock::from_time_t(time);
    //std::chrono::file_clock::time_point fileTime = std::chrono::clock_cast<std::chrono::file_clock>(sysTime);
    //std::filesystem::last_write_time(util::MakeNativeFileSystemPath(filePath), fileTime, ec);
    std::filesystem::last_write_time(util::MakeNativeFileSystemPath(filePath), time, ec);
    if (ec)
    {
        throw std::runtime_error("could not set write time of file '" + filePath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    hash = reader.ReadUtf8String();
    if (package)
    {
        package->IncrementFileContentPosition(size);
    }
}

std::string File::ComputeHash() const
{
    std::string filePath = Path(GetTargetRootDir());
    if (!std::filesystem::exists(util::MakeNativeFileSystemPath(filePath)))
    {
        throw std::runtime_error("file '" + filePath + "' does not exist");
    }
    util::Sha1 sha1;
    util::FileStream fileStream(filePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream bufferedStream(fileStream);
    int64_t n = size;
    for (int64_t i = 0; i < n; ++i)
    {
        int x = bufferedStream.ReadByte();
        if (x == -1)
        {
            throw std::runtime_error("unexpected end of file '" + filePath + "'");
        }
        uint8_t b = static_cast<uint8_t>(x);
        sha1.Process(b);
    }
    std::string h = sha1.GetDigest();
    return h;
}

void File::SetFlag(FileFlags flag, bool value)
{
    if (value) flags = flags | flag;
    else flags = flags & ~flag;
}

bool File::Changed() const
{
    std::string filePath = Path(GetTargetRootDir());
    if (std::filesystem::exists(util::MakeNativeFileSystemPath(filePath)))
    {
        if (size != std::filesystem::file_size(util::MakeNativeFileSystemPath(filePath)))
        {
            return true;
        }
        std::string h = ComputeHash();
        if (h != hash)
        {
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}

void File::Remove()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            std::string filePath = Path(GetTargetRootDir());
            std::error_code ec;
            std::filesystem::remove(util::MakeNativeFileSystemPath(filePath), ec);
            if (ec)
            {
                throw std::runtime_error("could not remove file '" + filePath + "': " + util::PlatformStringToUtf8(ec.message()));
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

void File::Uninstall()
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetFile(this);
        package->CheckInterrupted();
    }
    Node::Uninstall();
    if (!GetFlag(FileFlags::exists) && !Changed())
    {
        Remove();
    }
    if (package)
    {
        package->IncrementFileIndex();
    }
}

xml::Element* File::ToXml() const
{
    xml::Element* element = xml::MakeElement("file");
    element->SetAttribute("name", Name());
    element->SetAttribute("size", std::to_string(size));
    element->SetAttribute("time", util::FileTimeToString(time));
    element->SetAttribute("hash", hash);
    return element;
}

} // namespace wing_package

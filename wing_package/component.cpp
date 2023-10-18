// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.component;

import wing_package.package;
import wing_package.directory;
import wing_package.file;
import wing_package.path_matcher;
import wing_package.links;
import wing_package.environment;
import xpath;
import std.filesystem;

namespace wing_package {

std::string CompressionStr(Compression compression)
{
    switch (compression)
    {
    case Compression::none: return "none";
    case Compression::deflate: return "deflate";
    //case Compression::bzip2: return "bzip2";
    }
    return std::string();
}

Compression ParseCompressionStr(const std::string& compressionStr)
{
    if (compressionStr == "none") return Compression::none;
    else if (compressionStr == "deflate") return Compression::deflate;
    //else if (compressionStr == "bzip2") return Compression::bzip2;
    else throw std::runtime_error("invalid compression name");
}

Component::Component() : Node(NodeKind::component)
{
}

Component::Component(const std::string& name_) : Node(NodeKind::component, name_)
{
}

Component::Component(NodeKind nodeKind_, const std::string& name_) : Node(nodeKind_, name_)
{
}

Component::Component(PathMatcher& pathMatcher, xml::Element* element) : Node(NodeKind::component)
{
    std::string nameAttr = element->GetAttribute("name");
    if (!nameAttr.empty())
    {
        SetName(nameAttr);
    }
    else
    {
        throw std::runtime_error("component element has no 'name' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
    }
    pathMatcher.BeginFiles(element);
    std::vector<FileInfo> fileInfos = pathMatcher.Files();
    for (const auto& fileInfo : fileInfos)
    {
        File* file = new File(fileInfo.name);
        file->SetSize(fileInfo.size);
        file->SetTime(fileInfo.time);
        AddFile(file);
    }
    pathMatcher.EndFiles();
    std::unique_ptr<xpath::XPathObject> directoryObject = xpath::Evaluate("directory", element);
    if (directoryObject)
    {
        if (directoryObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(directoryObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string nameAttr = element->GetAttribute("name");
                    if (!nameAttr.empty())
                    {
                        //std::time_t time = std::time_t();
                        std::chrono::file_clock::time_point time;
                        Directory* directory = new Directory(pathMatcher, nameAttr, time, element);
                        AddDirectory(directory);
                    }
                    else
                    {
                        throw std::runtime_error("directory element has no 'name' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                }
            }
        }
    }
}

void Component::RunCommands()
{
}

void Component::CreateInstallationInfo()
{
}

void Component::RemoveInstallationInfo()
{
}

void Component::AddDirectory(Directory* directory)
{
    directory->SetParent(this);
    directories.push_back(std::unique_ptr<Directory>(directory));
}

void Component::AddFile(File* file)
{
    file->SetParent(this);
    files.push_back(std::unique_ptr<File>(file));
}

void Component::Write(util::Streams& streams)
{
}

void Component::Read(util::Streams& streams)
{
}

void Component::WriteIndex(util::BinaryStreamWriter& writer)
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
    }
    Node::WriteIndex(writer);
    int32_t numDirectories = directories.size();
    writer.Write(numDirectories);
    for (int32_t i = 0; i < numDirectories; ++i)
    {
        Directory* directory = directories[i].get();
        wing_package::WriteIndex(directory, writer);
    }
    int32_t numFiles = files.size();
    writer.Write(numFiles);
    for (int32_t i = 0; i < numFiles; ++i)
    {
        File* file = files[i].get();
        wing_package::WriteIndex(file, writer);
    }
}

void Component::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
    }
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

void Component::WriteData(util::BinaryStreamWriter& writer)
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

void Component::ReadData(util::BinaryStreamReader& reader)
{
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
    }
    for (const auto& directory : directories)
    {
        directory->ReadData(reader);
    }
    std::string directoryPath = GetTargetRootDir();
    bool exists = std::filesystem::exists(util::MakeNativeFileSystemPath(directoryPath));
    std::error_code ec;
    std::filesystem::create_directories(util::MakeNativeFileSystemPath(directoryPath), ec);
    if (ec)
    {
        throw std::runtime_error("could not create directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    for (const auto& file : files)
    {
        file->ReadData(reader);
    }
}

void Component::Uninstall()
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
}

xml::Element* Component::ToXml() const
{
    xml::Element* element = xml::MakeElement("component");
    element->SetAttribute("name", Name());
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

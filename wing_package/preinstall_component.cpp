// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.preinstall_component;

import wing_package.package;
import wing_package.file;
import wing_package.links;
import wing_package.environment;
import xpath;
import std.filesystem;

namespace wing_package {

PreinstallComponent::PreinstallComponent() : Component(NodeKind::preinstall_component, "preinstall")
{
}

PreinstallComponent::PreinstallComponent(PathMatcher& pathMatcher, xml::Element* element)
{
    std::unique_ptr<xpath::XPathObject> includeObject = xpath::Evaluate("include", element);
    if (includeObject)
    {
        if (includeObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(includeObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* includeElement = static_cast<xml::Element*>(node);
                    std::string filePathAttr = includeElement->GetAttribute("filePath");
                    if (!filePathAttr.empty())
                    {
                        std::string filePath = util::GetFullPath(util::Path::Combine(pathMatcher.SourceRootDir(), filePathAttr));
                        FileInfo fileInfo(filePath, std::filesystem::file_size(util::MakeNativeFileSystemPath(filePath)), 
                            std::filesystem::last_write_time(util::MakeNativeFileSystemPath(filePath)));
                        fileInfos.push_back(fileInfo);
                        File* file = new File(util::Path::GetFileName(filePath));
                        file->SetSize(fileInfo.size);
                        file->SetTime(fileInfo.time);
                        AddFile(file);
                    }
                    else
                    {
                        throw std::runtime_error("preinstall/include element has no 'filePath' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                            std::to_string(includeElement->GetSourcePos().line) + ", column " + std::to_string(includeElement->GetSourcePos().col));
                    }
                }
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> runObject = xpath::Evaluate("run", element);
    if (runObject)
    {
        if (runObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(runObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string commandAttr = element->GetAttribute("command");
                    if (!commandAttr.empty())
                    {
                        commands.push_back(commandAttr);
                    }
                    else
                    {
                        throw std::runtime_error("preinstall/run element has no 'command' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                }
            }
        }
    }
}

void PreinstallComponent::Write(util::Streams& streams)
{
    util::BinaryStreamWriter writer(streams.Back());
    WriteIndex(writer);
    WriteData(writer);
}

void PreinstallComponent::Read(util::Streams& streams)
{
    util::BinaryStreamReader reader(streams.Back());
    ReadIndex(reader);
    ReadData(reader);
}

void PreinstallComponent::RunCommands()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            int n = commands.size();
            for (int i = 0; i < n; ++i)
            {
                const std::string& command = commands[i];
                int exitCode = 0;
                try
                {
                    util::Process process(command, util::Process::Redirections::none);
                    process.WaitForExit();
                    exitCode = process.ExitCode();
                }
                catch (const std::exception& ex)
                {
                    throw std::runtime_error("error running preinstall command '" + command + "': " + ex.what());
                }
                if (exitCode != 0)
                {
                    throw std::runtime_error("error: preinstall command '" + command + "' returned nonzero exit code " + std::to_string(exitCode));
                }
            }
        }
        catch (const std::exception&)
        {
            RemovePreinstallDir();
            throw;
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
    RemovePreinstallDir();
}

void PreinstallComponent::AddFile(File* file)
{
    file->SetParent(this);
    files.push_back(std::unique_ptr<File>(file));
}

void PreinstallComponent::WriteIndex(util::BinaryStreamWriter& writer)
{
    Component::WriteIndex(writer);
    int32_t numFiles = files.size();
    writer.Write(numFiles);
    for (int32_t i = 0; i < numFiles; ++i)
    {
        File* file = files[i].get();
        wing_package::WriteIndex(file, writer);
    }
    int32_t numCommands = commands.size();
    writer.Write(numCommands);
    for (int32_t i = 0; i < numCommands; ++i)
    {
        writer.Write(commands[i]);
    }
}

void PreinstallComponent::ReadIndex(util::BinaryStreamReader& reader)
{
    Component::ReadIndex(reader);
    std::string preinstallDir;
    Package* package = GetPackage();
    if (package)
    {
        package->SetComponent(this);
        package->CheckInterrupted();
        preinstallDir = package->PreinstallDir();
        if (preinstallDir.empty())
        {
            throw std::runtime_error("preinstall directory not set");
        }
        std::error_code ec;
        std::filesystem::create_directories(util::MakeNativeFileSystemPath(preinstallDir), ec);
        if (ec)
        {
            throw std::runtime_error("could not create preinstall directory '" + preinstallDir + "': " + util::PlatformStringToUtf8(ec.message()));
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
    int32_t numFiles = reader.ReadInt();
    for (int32_t i = 0; i < numFiles; ++i)
    {
        File* file = wing_package::BeginReadFile(reader);
        AddFile(file);
        file->ReadIndex(reader);
    }
    int32_t numCommands = reader.ReadInt();
    for (int32_t i = 0; i < numCommands; ++i)
    {
        std::string command = reader.ReadUtf8String();
        commands.push_back(package->ExpandPath(command));
    }
}

void PreinstallComponent::WriteData(util::BinaryStreamWriter& writer)
{
    int32_t n = files.size();
    for (int32_t i = 0; i < n; ++i)
    {
        const FileInfo& fileInfo = fileInfos[i];
        std::string filePath = fileInfo.name; // file info 'name' contains actually full file path
        util::FileStream file(filePath, util::OpenMode::read | util::OpenMode::binary);
        util::BufferedStream bufferedFile(file);
        util::BinaryStreamReader reader(bufferedFile);
        int64_t size = fileInfo.size;
        for (int64_t i = 0; i < size; ++i)
        {
            uint8_t x = reader.ReadByte();
            writer.Write(x);
        }
    }
}

void PreinstallComponent::ReadData(util::BinaryStreamReader& reader)
{
    Package* package = GetPackage();
    std::string preinstallDir;
    if (package)
    {
        package->CheckInterrupted();
        preinstallDir = package->PreinstallDir();
    }
    else
    {
        throw std::runtime_error("package not set");
    }
    int32_t n = files.size();
    for (int32_t i = 0; i < n; ++i)
    {
        File* file = files[i].get();
        std::string filePath = util::Path::Combine(preinstallDir, file->Name());
        {
            util::FileStream fileStream(filePath, util::OpenMode::write | util::OpenMode::binary);
            util::BufferedStream bufferedStream(fileStream);
            int64_t size = file->Size();
            for (int64_t i = 0; i < size; ++i)
            {
                uint8_t x = reader.ReadByte();
                bufferedStream.Write(x);
            }
        }
    }
}

void PreinstallComponent::RemovePreinstallDir()
{
    try
    {
        Package* package = GetPackage();
        if (package)
        {
            std::filesystem::remove_all(util::MakeNativeFileSystemPath(package->PreinstallDir()));
        }
    }
    catch (...)
    {
    }
}

} // namespace wing_package

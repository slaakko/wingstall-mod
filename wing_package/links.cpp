// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.links;

import wing_package.package;
import wing_package.path_matcher;
import wing_package.environment;
import wing_package.file;
import wing;
import xpath;
import util;
import std.filesystem;

namespace wing_package {

Link::Link() : Node(NodeKind::link, "link"), iconIndex(0)
{
}

Link::Link(const std::string& linkFilePath_, const std::string& path_, const std::string& arguments_, const std::string& workingDirectory_,
    const std::string& description_, const std::string& iconPath_, int iconIndex_) :
    Node(NodeKind::link, "link"),
    linkFilePath(linkFilePath_), path(path_), arguments(arguments_), workingDirectory(workingDirectory_), description(description_), iconPath(iconPath_), iconIndex(iconIndex_)
{
}

Link::Link(PathMatcher& pathMatcher, xml::Element* element) : Node(NodeKind::link, "link"), iconIndex(0)
{
    std::string linkFilePathAttr = element->GetAttribute("linkFilePath");
    if (!linkFilePathAttr.empty())
    {
        linkFilePath = linkFilePathAttr;
    }
    else
    {
        throw std::runtime_error("link element has no 'linkFilePath' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
    }
    std::string pathAttr = element->GetAttribute("path");
    if (!pathAttr.empty())
    {
        path = pathAttr;
    }
    else
    {
        throw std::runtime_error("link element has no 'path' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
    }
    std::string argumentsAttr = element->GetAttribute("arguments");
    if (!argumentsAttr.empty())
    {
        arguments = argumentsAttr;
    }
    std::string workingDirectoryAttr = element->GetAttribute("workingDirectory");
    if (!workingDirectoryAttr.empty())
    {
        workingDirectory = workingDirectoryAttr;
    }
    std::string descriptionAttr = element->GetAttribute("description");
    if (!descriptionAttr.empty())
    {
        description = descriptionAttr;
    }
    std::string iconPathAttr = element->GetAttribute("iconPath");
    if (!iconPathAttr.empty())
    {
        iconPath = iconPathAttr;
    }
    std::string iconIndexAttr = element->GetAttribute("iconIndex");
    if (!iconIndexAttr.empty())
    {
        iconIndex = std::stoi(iconIndexAttr);
    }
}

void Link::SetOld(Link* old_)
{
    old.reset(old_);
    old->SetParent(this);
}

void Link::WriteIndex(util::BinaryStreamWriter& writer)
{
    Node::WriteIndex(writer);
    writer.Write(linkFilePath);
    writer.Write(expandedLinkFilePath);
    writer.Write(path);
    writer.Write(expandedPath);
    writer.Write(arguments);
    writer.Write(workingDirectory);
    writer.Write(expandedWorkingDirectory);
    writer.Write(description);
    writer.Write(iconPath);
    writer.Write(expandedIconPath);
    writer.Write(iconIndex);
    bool hasOld = old != nullptr;
    writer.Write(hasOld);
    if (hasOld)
    {
        old->WriteIndex(writer);
    }
}

void Link::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    linkFilePath = reader.ReadUtf8String();
    expandedLinkFilePath = reader.ReadUtf8String();
    path = reader.ReadUtf8String();
    expandedPath = reader.ReadUtf8String();
    arguments = reader.ReadUtf8String();
    workingDirectory = reader.ReadUtf8String();
    expandedWorkingDirectory = reader.ReadUtf8String();
    description = reader.ReadUtf8String();
    iconPath = reader.ReadUtf8String();
    expandedIconPath = reader.ReadUtf8String();
    iconIndex = reader.ReadInt();
    bool hasOld = reader.ReadBool();
    if (hasOld)
    {
        SetOld(new Link());
        old->ReadIndex(reader);
    }
}

xml::Element* Link::ToXml() const
{
    xml::Element* element = xml::MakeElement("link");
    element->SetAttribute("linkFilePath", linkFilePath);
    element->SetAttribute("path", path);
    element->SetAttribute("arguments", arguments);
    element->SetAttribute("workingDirecory", workingDirectory);
    element->SetAttribute("description", description);
    element->SetAttribute("iconPath", iconPath);
    element->SetAttribute("iconIndex", std::to_string(iconIndex));
    return element;
}

void Link::Create(const std::string& expandedLinkFilePath, const std::string& expandedPath, const std::string& expandedWorkingDirectory, const std::string& expandedIconPath)
{
    try
    {
        std::string directoryPath = util::Path::GetDirectoryName(util::GetFullPath(expandedLinkFilePath));
        std::error_code ec;
        std::filesystem::create_directories(util::MakeNativeFileSystemPath(directoryPath), ec);
        if (ec)
        {
            throw std::runtime_error("could not create directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
        }
        wing::CreateShellLink(expandedLinkFilePath, expandedPath, arguments, expandedWorkingDirectory, description, expandedIconPath, iconIndex);
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("could not create link '" + expandedLinkFilePath + "': " + ex.what());
    }
}

void Link::Install()
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        expandedLinkFilePath = package->ExpandPath(linkFilePath);
        expandedPath = package->ExpandPath(path);
        expandedWorkingDirectory.clear();
        if (!workingDirectory.empty())
        {
            expandedWorkingDirectory = package->ExpandPath(workingDirectory);
        }
        expandedIconPath.clear();
        if (!iconPath.empty())
        {
            expandedIconPath = package->ExpandPath(iconPath);
        }
        if (std::filesystem::exists(util::MakeNativeFileSystemPath(expandedLinkFilePath)))
        {
            try
            {
                std::string p;
                std::string args;
                std::string wdir;
                std::string desc;
                std::string iconP;
                int index = 0;
                wing::GetShellLinkData(expandedLinkFilePath, p, args, wdir, desc, iconP, index);
                SetOld(new Link(expandedLinkFilePath, p, args, wdir, desc, iconP, index));
            }
            catch (const std::exception& ex)
            {
                package->LogError("could not get old link data from file '" + expandedLinkFilePath + "': " + ex.what());
            }
        }
        Create(expandedLinkFilePath, expandedPath, expandedWorkingDirectory, expandedIconPath);
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void Link::Uninstall()
{
    Node::Uninstall();
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        try
        {
            if (std::filesystem::exists(util::MakeNativeFileSystemPath(expandedLinkFilePath)))
            {
                std::error_code ec;
                std::filesystem::remove(util::MakeNativeFileSystemPath(expandedLinkFilePath), ec);
                if (ec)
                {
                    throw std::runtime_error(util::PlatformStringToUtf8(ec.message()));
                }
            }
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not remove link file '" + expandedLinkFilePath + "': " + ex.what());
        }
        try
        {
            if (old)
            {
                old->Create(old->LinkFilePath(), old->Path(), old->WorkingDirectory(), old->IconPath());
            }
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not recreate old link '" + old->LinkFilePath() + "': " + ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

LinkDirectory::LinkDirectory() : Node(NodeKind::linkDirectory, "linkDirectory"), flags(LinkDirectoryFlags::none)
{
}

LinkDirectory::LinkDirectory(const std::string& path_) : Node(NodeKind::linkDirectory, "linkDirectory"), path(path_), flags(LinkDirectoryFlags::none)
{
}

LinkDirectory::LinkDirectory(PathMatcher& pathMatcher, xml::Element* element) : Node(NodeKind::linkDirectory, "linkDirectory"), flags(LinkDirectoryFlags::none)
{
    std::string pathAttr = element->GetAttribute("path");
    if (!pathAttr.empty())
    {
        path = pathAttr;
    }
    else
    {
        throw std::runtime_error("link directory element has no 'path' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
    }
}

void LinkDirectory::WriteIndex(util::BinaryStreamWriter& writer)
{
    Node::WriteIndex(writer);
    writer.Write(path);
    writer.Write(expandedPath);
    writer.Write(static_cast<uint8_t>(flags));
}

void LinkDirectory::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    path = reader.ReadUtf8String();
    expandedPath = reader.ReadUtf8String();
    flags = static_cast<LinkDirectoryFlags>(reader.ReadByte());
}

xml::Element* LinkDirectory::ToXml() const
{
    xml::Element* element = xml::MakeElement("linkDirectory");
    element->SetAttribute("path", path);
    return element;
}

void LinkDirectory::SetFlag(LinkDirectoryFlags flag, bool value)
{
    if (value) flags = flags | flag;
    else flags = flags & ~flag;
}

void LinkDirectory::Create()
{
    std::error_code ec;
    std::filesystem::create_directories(util::MakeNativeFileSystemPath(expandedPath), ec);
    if (ec)
    {
        throw std::runtime_error("could not create directory '" + expandedPath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
}

void LinkDirectory::Remove()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            std::error_code ec;
            std::filesystem::remove(util::MakeNativeFileSystemPath(expandedPath), ec);
            if (ec)
            {
                throw std::runtime_error("could not remove directory '" + expandedPath + "': " + util::PlatformStringToUtf8(ec.message()));
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

void LinkDirectory::Install()
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        expandedPath = package->ExpandPath(path);
        std::u16string expPath = util::ToUtf16(expandedPath);
        std::filesystem::path p((const wchar_t*)expPath.c_str());
        SetFlag(LinkDirectoryFlags::exists, std::filesystem::exists(p));
        if (!GetFlag(LinkDirectoryFlags::exists))
        {
            Create();
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void LinkDirectory::Uninstall()
{
    Node::Uninstall();
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        if (!GetFlag(LinkDirectoryFlags::exists))
        {
            Remove();
        }
    }
}

Links::Links() : Node(NodeKind::links, "links")
{
}

Links::Links(PathMatcher& pathMatcher, xml::Element* element) : Node(NodeKind::links, "links")
{
    std::unique_ptr<xpath::XPathObject> linkDirectoryObject = xpath::Evaluate("linkDirectory", element);
    if (linkDirectoryObject)
    {
        if (linkDirectoryObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(linkDirectoryObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    LinkDirectory* linkDirectory = new LinkDirectory(pathMatcher, element);
                    AddLinkDirectory(linkDirectory);
                }
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> linkObject = xpath::Evaluate("link", element);
    if (linkObject)
    {
        if (linkObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(linkObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    Link* link = new Link(pathMatcher, element);
                    AddLink(link);
                }
            }
        }
    }
}

void Links::AddLinkDirectory(LinkDirectory* linkDirectory)
{
    linkDirectory->SetParent(this);
    linkDirectories.push_back(std::unique_ptr<LinkDirectory>(linkDirectory));
}

void Links::AddLink(Link* link)
{
    link->SetParent(this);
    links.push_back(std::unique_ptr<Link>(link));
}

xml::Element* Links::ToXml() const
{
    xml::Element* element = xml::MakeElement("links");
    for (const auto& linkDirectory : linkDirectories)
    {
        xml::Element* child = linkDirectory->ToXml();
        element->AppendChild(child);
    }
    for (const auto& link : links)
    {
        xml::Element* child = link->ToXml();
        element->AppendChild(child);
    }
    return element;
}

void Links::WriteIndex(util::BinaryStreamWriter& writer)
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
    }
    Node::WriteIndex(writer);
    int32_t numLinkDirectories = linkDirectories.size();
    writer.Write(numLinkDirectories);
    for (int32_t i = 0; i < numLinkDirectories; ++i)
    {
        LinkDirectory* linkDirectory = linkDirectories[i].get();
        linkDirectory->WriteIndex(writer);
    }
    int32_t numLinks = links.size();
    writer.Write(numLinks);
    for (int32_t i = 0; i < numLinks; ++i)
    {
        Link* link = links[i].get();
        link->WriteIndex(writer);
    }
}

void Links::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    int32_t numLinkDirectories = reader.ReadInt();
    for (int32_t i = 0; i < numLinkDirectories; ++i)
    {
        LinkDirectory* linkDirectory = new LinkDirectory();
        AddLinkDirectory(linkDirectory);
        linkDirectory->ReadIndex(reader);
    }
    int32_t numLinks = reader.ReadInt();
    for (int32_t i = 0; i < numLinks; ++i)
    {
        Link* link = new Link();
        AddLink(link);
        link->ReadIndex(reader);
    }
}

void Links::Install()
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        for (const auto& linkDirectory : linkDirectories)
        {
            linkDirectory->Install();
        }
        for (const auto& link : links)
        {
            link->Install();
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void Links::Uninstall()
{
    Node::Uninstall();
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        for (const auto& link : links)
        {
            link->Uninstall();
        }
        for (const auto& linkDirectory : linkDirectories)
        {
            linkDirectory->Uninstall();
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

} // namespace wing_package

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.environment;

import wing_package.package;
import wing_package.path_matcher;
import wing_package.links;
import wing_package.file;
import wing;
import xpath;
import util;

namespace wing_package {

EnvironmentVariable::EnvironmentVariable() : Node(NodeKind::environmentVariable), flags(EnvironmentVariableFlags::none)
{
}

EnvironmentVariable::EnvironmentVariable(const std::string& name_, const std::string& value_) : Node(NodeKind::environmentVariable, name_), value(value_), flags(EnvironmentVariableFlags::none)
{
}

EnvironmentVariable::EnvironmentVariable(PathMatcher& pathMatcher, xml::Element* element) : Node(NodeKind::environmentVariable), flags(EnvironmentVariableFlags::none)
{
    std::string nameAttr = element->GetAttribute("name");
    if (!nameAttr.empty())
    {
        SetName(nameAttr);
    }
    else
    {
        throw std::runtime_error("environment variable element has no 'name' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
    }
    std::string valueAttr = element->GetAttribute("value");
    if (!valueAttr.empty())
    {
        value = valueAttr;
    }
    else
    {
        throw std::runtime_error("environment variable element has no 'value' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
    }
}

xml::Element* EnvironmentVariable::ToXml() const
{
    xml::Element* element = xml::MakeElement("variable");
    element->SetAttribute("name", Name());
    element->SetAttribute("value", value);
    element->SetAttribute("oldValue", oldValue);
    return element;
}

void EnvironmentVariable::SetFlag(EnvironmentVariableFlags flag, bool value)
{
    if (value) flags = flags | flag;
    else flags = flags & ~flag;
}

void EnvironmentVariable::WriteIndex(util::BinaryStreamWriter& writer)
{
    Node::WriteIndex(writer);
    writer.Write(value);
    writer.Write(oldValue);
    writer.Write(static_cast<uint8_t>(flags));
}

void EnvironmentVariable::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    value = reader.ReadUtf8String();
    oldValue = reader.ReadUtf8String();
    flags = static_cast<EnvironmentVariableFlags>(reader.ReadByte());
}

void EnvironmentVariable::Install()
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        try
        {
            SetFlag(EnvironmentVariableFlags::exists, wing::HasSystemEnvironmentVariable(Name()));
            if (GetFlag(EnvironmentVariableFlags::exists))
            {
                oldValue = wing::GetSystemEnvironmentVariable(Name());
            }
        }
        catch (const std::exception& ex)
        {
            throw std::runtime_error("could not get value of environment variable '" + Name() + "': " + ex.what());
        }
        std::string path = package->ExpandPath(value);
        try
        {
            wing::SetSystemEnvironmentVariable(Name(), path, wing::RegistryValueKind::regSz);
        }
        catch (const std::exception& ex)
        {
            throw std::runtime_error("could not create environment variable '" + Name() + "': " + ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void EnvironmentVariable::SetOldValue()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            wing::SetSystemEnvironmentVariable(Name(), oldValue, wing::RegistryValueKind::regSz);
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not set old value of environment variable '" + Name() + "': " + ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void EnvironmentVariable::Remove()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            wing::DeleteSystemEnvironmentVariable(Name());
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not remove environment variable '" + Name() + "': " + ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void EnvironmentVariable::Uninstall()
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        Node::Uninstall();
        if (GetFlag(EnvironmentVariableFlags::exists) && !oldValue.empty())
        {
            SetOldValue();
        }
        else
        {
            Remove();
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

PathDirectory::PathDirectory() : Node(NodeKind::pathDirectory), flags(PathDirectoryFlags::none)
{
}

PathDirectory::PathDirectory(const std::string& value_) : Node(NodeKind::pathDirectory), value(value_), flags(PathDirectoryFlags::none)
{
}

PathDirectory::PathDirectory(PathMatcher& pathMatcher, xml::Element* element) : Node(NodeKind::pathDirectory), flags(PathDirectoryFlags::none)
{
    std::string valueAttr = element->GetAttribute("value");
    if (!valueAttr.empty())
    {
        value = valueAttr;
    }
    else
    {
        throw std::runtime_error("path directory element has no 'value' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
    }
}

xml::Element* PathDirectory::ToXml() const
{
    xml::Element* element = xml::MakeElement("pathDirectory");
    element->SetAttribute("value", value);
    return element;
}

void PathDirectory::WriteIndex(util::BinaryStreamWriter& writer)
{
    Node::WriteIndex(writer);
    writer.Write(value);
    writer.Write(expandedValue);
    writer.Write(static_cast<uint8_t>(flags));
}

void PathDirectory::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    value = reader.ReadUtf8String();
    expandedValue = reader.ReadUtf8String();
    flags = static_cast<PathDirectoryFlags>(reader.ReadByte());
}

void PathDirectory::Install()
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        expandedValue = package->ExpandPath(value);
        SetFlag(PathDirectoryFlags::exists, wing::HasPathDirectory(expandedValue));
        if (!GetFlag(PathDirectoryFlags::exists))
        {
            wing::AppendPathDirectory(expandedValue);
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void PathDirectory::Remove()
{
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            wing::RemovePathDirectory(expandedValue);
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not remove directory '" + expandedValue + "' from PATH environment variable :" + ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void PathDirectory::Uninstall()
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        Node::Uninstall();
        if (!GetFlag(PathDirectoryFlags::exists))
        {
            Remove();
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void PathDirectory::SetFlag(PathDirectoryFlags flag, bool value)
{
    if (value) flags = flags | flag;
    else flags = flags & ~flag;
}

Environment::Environment() : Node(NodeKind::environment, "environment")
{
}

Environment::Environment(PathMatcher& pathMatcher, xml::Element* element) : Node(NodeKind::environment, "environment")
{
    std::unique_ptr<xpath::XPathObject> variableObject = xpath::Evaluate("variable", element);
    if (variableObject)
    {
        if (variableObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(variableObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    EnvironmentVariable* variable = new EnvironmentVariable(pathMatcher, element);
                    AddVariable(variable);
                }
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> pathDirectoryObject = xpath::Evaluate("pathDirectory", element);
    if (pathDirectoryObject)
    {
        if (pathDirectoryObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(pathDirectoryObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    PathDirectory* pathDirectory = new PathDirectory(pathMatcher, element);
                    AddPathDirectory(pathDirectory);
                }
            }
        }
    }
}

void Environment::AddVariable(EnvironmentVariable* variable)
{
    variable->SetParent(this);
    variables.push_back(std::unique_ptr<EnvironmentVariable>(variable));
}

void Environment::AddPathDirectory(PathDirectory* pathDirectory)
{
    pathDirectory->SetParent(this);
    pathDirectories.push_back(std::unique_ptr<PathDirectory>(pathDirectory));
}

void Environment::WriteIndex(util::BinaryStreamWriter& writer)
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
    }
    Node::WriteIndex(writer);
    int32_t numVariables = variables.size();
    writer.Write(numVariables);
    for (int32_t i = 0; i < numVariables; ++i)
    {
        EnvironmentVariable* variable = variables[i].get();
        variable->WriteIndex(writer);
    }
    int32_t numPathDirectories = pathDirectories.size();
    writer.Write(numPathDirectories);
    for (int32_t i = 0; i < numPathDirectories; ++i)
    {
        PathDirectory* pathDirectory = pathDirectories[i].get();
        pathDirectory->WriteIndex(writer);
    }
}

void Environment::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    int32_t numVariables = reader.ReadInt();
    for (int32_t i = 0; i < numVariables; ++i)
    {
        EnvironmentVariable* variable = new EnvironmentVariable();
        AddVariable(variable);
        variable->ReadIndex(reader);
    }
    int32_t numPathDirectories = reader.ReadInt();
    for (int32_t i = 0; i < numPathDirectories; ++i)
    {
        PathDirectory* pathDirectory = new PathDirectory();
        AddPathDirectory(pathDirectory);
        pathDirectory->ReadIndex(reader);
    }
}

xml::Element* Environment::ToXml() const
{
    xml::Element* element = xml::MakeElement("environment");
    for (const auto& variable : variables)
    {
        xml::Element* child = variable->ToXml();
        element->AppendChild(child);
    }
    for (const auto& pathDirectory : pathDirectories)
    {
        xml::Element* child = pathDirectory->ToXml();
        element->AppendChild(child);
    }
    return element;
}

void Environment::Install()
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
        for (const auto& variable : variables)
        {
            variable->Install();
        }
        for (const auto& pathDirectory : pathDirectories)
        {
            pathDirectory->Install();
        }
        try
        {
            wing::BroadcastEnvironmentChangedMessage();
        }
        catch (const std::exception& ex)
        {
            std::string what = ex.what();
            int x = 0;
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void Environment::Uninstall()
{
    Node::Uninstall();
    Package* package = GetPackage();
    if (package)
    {
        try
        {
            package->CheckInterrupted();
            for (const auto& variable : variables)
            {
                variable->Uninstall();
            }
            for (const auto& pathDirectory : pathDirectories)
            {
                pathDirectory->Uninstall();
            }
            try
            {
                wing::BroadcastEnvironmentChangedMessage();
            }
            catch (const std::exception& ex)
            {
                package->LogError(ex.what());
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

} // namespace wing_package

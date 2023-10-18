// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.node;

import dom;
import util;
import std.core;

export namespace wing_package {

enum class NodeKind : uint8_t
{
    package, component, directory, file, environment, environmentVariable, pathDirectory, variables, variable, links, linkDirectory, link, preinstall_component,
    uninstall_component, uninstall_exe_file, uninstall_bin_file, installation_component
};

class Component;
class Directory;
class File;

class Package;

class Node
{
public:
    Node(NodeKind kind_);
    Node(NodeKind kind_, const std::string& name_);
    virtual ~Node();
    NodeKind Kind() const { return kind; }
    const std::string& Name() const { return name; }
    void SetName(const std::string& name_);
    std::string FullName() const;
    std::string Path() const;
    std::string Path(const std::string& root) const;
    Node* Parent() const { return parent; }
    void SetParent(Node* parent_) { parent = parent_; }
    virtual Package* GetPackage() const;
    virtual const std::string& GetSourceRootDir() const;
    virtual const std::string& GetTargetRootDir() const;
    virtual void WriteIndex(util::BinaryStreamWriter& writer);
    virtual void ReadIndex(util::BinaryStreamReader& reader);
    virtual void WriteData(util::BinaryStreamWriter& writer);
    virtual void ReadData(util::BinaryStreamReader& reader);
    virtual void Uninstall();
    virtual xml::Element* ToXml() const = 0;
private:
    NodeKind kind;
    std::string name;
    Node* parent;
};

Node* CreateNode(NodeKind kind);
void WriteIndex(Node* node, util::BinaryStreamWriter& writer);
Component* BeginReadComponent(util::BinaryStreamReader& reader);
Directory* BeginReadDirectory(util::BinaryStreamReader& reader);
File* BeginReadFile(util::BinaryStreamReader& reader);

} // namespace wing_package

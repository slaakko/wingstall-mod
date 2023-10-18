// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.component;

import package_editor.directory;
import package_editor.file;
import package_editor.node;
import wing;
import dom;
import std.core;

export namespace package_editor {

class Component;

class Components : public Node
{
public:
    Components();
    void AddElements(xml::Element* packageElement);
    void AddComponent(Component* component);
    const std::vector<std::unique_ptr<Component>>& GetComponents() const { return components; }
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    std::string ImageName() const override { return "components.bitmap"; }
    std::string ViewName() const override { return "components_view"; }
    int Count() const override;
    int IndexOf(const Node* child) const override;
    Node* GetNode(int index) const override;
    bool HasNode(const std::string& name) const override;
    std::unique_ptr<Node> RemoveChild(int index) override;
    void InsertBefore(int index, std::unique_ptr<Node>&& child);
    void InsertAfter(int index, std::unique_ptr<Node>&& child);
    bool CanAdd() const override { return true; }
    void AddNew(NodeKind kind) override;
    void AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions) override;
    void MakeDisjoint();
    void MakeMaps();
    Component* GetDirectoryComponent(const std::u32string& directoryName) const;
    Component* GetFileComponent(const std::u32string& fileName) const;
    void MapDirectoryComponent(const std::u32string& directoryName, Component* component);
    void RemoveDirectoryName(const std::u32string& directoryName);
    void MapFileComponent(const std::u32string& fileName, Component* component);
    void RemoveFileName(const std::u32string& fileName);
    void RemoveUnexistingDirectoriesAndFiles();
private:
    std::vector<std::unique_ptr<Component>> components;
    std::map<std::u32string, Component*> directoryNameComponentMap;
    std::map<std::u32string, Component*> fileNameComponentMap;
};

class Component : public Node
{
public:
    Component(const std::string& name_);
    Component(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    void AddDirectory(Directory* directory);
    void AddFile(File* file);
    std::string ViewName() const override { return "component_view"; }
    std::string ImageName() const override { return "component.bitmap"; }
    int Count() const override;
    int IndexOf(const Node* child) const override;
    Node* GetNode(int index) const override;
    std::unique_ptr<Node> RemoveChild(int index) override;
    void InsertBefore(int index, std::unique_ptr<Node>&& child);
    void InsertAfter(int index, std::unique_ptr<Node>&& child);
    bool CanAdd() const override { return true; }
    bool CanEdit() const override { return true; }
    bool CanMoveUp(const Node* child) const override;
    bool CanMoveDown(const Node* child) const override;
    void Edit() override;
    void AddNew(NodeKind kind) override;
    void AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions) override;
    std::vector<std::u32string> DirectoryNames() const;
    std::vector<std::u32string> FileNames() const;
    void FilterDirectories(const std::vector<std::u32string>& exclude);
    void FilterFiles(const std::vector<std::u32string>& exclude);
    void Map(const std::vector<std::u32string>& directoryNames, const std::vector<std::u32string>& fileNames);
    void Sort();
    void RemoveUnexistingDirectoriesAndFiles();
private:
    void AddDirectoriesAndFiles(const std::vector<std::u32string>& directoryNames, const std::vector<std::u32string>& fileNames);
    std::vector<std::unique_ptr<Directory>> directories;
    std::vector<std::unique_ptr<File>> files;
};

} // package_editor

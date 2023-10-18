// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.environment;

import package_editor.node;
import dom;
import wing;
import std.core;

export namespace package_editor {

class EnvironmentVariable;
class PathDirectory;

class Environment : public Node
{
public:
    Environment();
    Environment(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    std::string ImageName() const override { return "environment.bitmap"; }
    std::string ViewName() const override { return "environment_view"; }
    void AddEnvironentVariable(EnvironmentVariable* environmentVariable);
    void AddPathDirectory(PathDirectory* pathDirectory);
    int Count() const override;
    int IndexOf(const Node* child) const override;
    Node* GetNode(int index) const override;
    std::unique_ptr<Node> RemoveChild(int index) override;
    void InsertBefore(int index, std::unique_ptr<Node>&& child);
    void InsertAfter(int index, std::unique_ptr<Node>&& child);
    bool CanMoveUp(const Node* child) const override;
    bool CanMoveDown(const Node* child) const override;
    bool HasEnvironmentVariable(const std::string& name) const;
    bool HasPathDirectory(const std::string& value) const;
    bool CanAdd() const override { return true; }
    void AddNew(NodeKind kind) override;
    void AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions) override;
private:
    std::vector<std::unique_ptr<EnvironmentVariable>> environmentVariables;
    std::vector<std::unique_ptr<PathDirectory>> pathDirectories;
};

class EnvironmentVariable : public Node
{
public:
    EnvironmentVariable();
    EnvironmentVariable(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    const std::string& Value() const { return value; }
    void SetValue(const std::string& value_);
    std::string ImageName() const override { return "environment.var.bitmap"; }
    void SetData(wing::ListViewItem* item, wing::ImageList* imageList) override;
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    bool Leaf() const override { return true; }
    bool CanOpen() const override { return false; }
    bool CanEdit() const override { return true; }
    void Edit() override;
    void ExecuteDefaultAction() override { Edit(); }
private:
    std::string value;
};

class PathDirectory : public Node
{
public:
    PathDirectory();
    PathDirectory(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    const std::string& Value() const { return value; }
    void SetValue(const std::string& value_);
    std::string ImageName() const override { return "path.directory.bitmap"; }
    void SetData(wing::ListViewItem* item, wing::ImageList* imageList) override;
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    bool Leaf() const override { return true; }
    bool CanOpen() const override { return false; }
    bool CanEdit() const override { return true; }
    void Edit() override;
    void ExecuteDefaultAction() override { Edit(); }
private:
    std::string value;
};

} // package_editor


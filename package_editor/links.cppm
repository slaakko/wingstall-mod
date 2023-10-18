// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.links;

import package_editor.node;
import dom;
import std.core;

export namespace package_editor {

class LinkDirectory;
class LinkDirectories;
class Shortcuts;
class Shortcut;

class Links : public Node
{
public:
    Links();
    Links(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    std::string ImageName() const override { return "links.bitmap"; }
    std::string ViewName() const override { return "links_view"; }
private:
    std::unique_ptr<LinkDirectories> linkDirectories;
    std::unique_ptr<Shortcuts> shortcuts;
};

class LinkDirectories : public Node
{
public:
    LinkDirectories();
    void AddElements(xml::Element* parentElement);
    void AddLinkDirectory(LinkDirectory* linkDirectory);
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    std::string ImageName() const override { return "linked.folder.closed.bitmap"; }
    std::string ViewName() const override { return "link_directories_view"; }
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
private:
    std::vector<std::unique_ptr<LinkDirectory>> linkDirectories;
};

class LinkDirectory : public Node
{
public:
    LinkDirectory();
    LinkDirectory(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    const std::string& Path() const { return path; }
    void SetPath(const std::string& path_);
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    std::string ImageName() const override { return"linked.folder.closed.bitmap"; }
    void SetData(wing::ListViewItem* item, wing::ImageList* imageList) override;
    bool Leaf() const override { return true; }
    bool CanOpen() const override { return false; }
    bool CanEdit() const override { return true; }
    void Edit() override;
    void ExecuteDefaultAction() override { Edit(); }
private:
    std::string path;
};

class Shortcuts : public Node
{
public:
    Shortcuts();
    void AddElements(xml::Element* parentElement);
    void AddShortcut(Shortcut* shortcut);
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    std::string ImageName() const override { return "shortcut.bitmap"; }
    std::string ViewName() const override { return "shortcuts_view"; }
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
private:
    std::vector<std::unique_ptr<Shortcut>> shortcuts;
};

class Shortcut : public Node
{
public:
    Shortcut();
    Shortcut(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    std::string ImageName() const override { return"shortcut.bitmap"; }
    void SetData(wing::ListViewItem* item, wing::ImageList* imageList) override;
    const std::string& LinkFilePath() const { return linkFilePath; }
    void SetLinkFilePath(const std::string& linkFilePath_);
    const std::string& Path() const { return path; }
    void SetPath(const std::string& path_);
    const std::string& Arguments() const { return arguments; }
    void SetArguments(const std::string& arguments_);
    const std::string& WorkingDirectory() const { return workingDirectory; }
    void SetWorkingDirectory(const std::string& workingDirectory_);
    const std::string& Description() const { return description; }
    void SetDescription(const std::string& description_);
    const std::string& IconFilePath() const { return iconFilePath; }
    void SetIconFilePath(const std::string& iconFilePath_);
    int IconIndex() const { return iconIndex; }
    void SetIconIndex(int iconIndex_) { iconIndex = iconIndex_; }
    bool Leaf() const override { return true; }
    bool CanOpen() const override { return false; }
    bool CanEdit() const override { return true; }
    void Edit() override;
    void ExecuteDefaultAction() override { Edit(); }
private:
    std::string linkFilePath;
    std::string path;
    std::string arguments;
    std::string workingDirectory;
    std::string description;
    std::string iconFilePath;
    int iconIndex;
};

} // package_editor

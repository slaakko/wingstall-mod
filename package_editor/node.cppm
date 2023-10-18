// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.node;

import wing;
import wing_package;
import dom;
import std.core;

export namespace package_editor {

void GetDirectoriesAndFiles(const std::string& dirPath, std::vector<std::u32string>& directories, std::vector<std::u32string>& files);
std::vector<std::u32string> Filter(const std::vector<std::u32string>& names, const std::vector<std::u32string>& exclude);
std::vector<std::u32string> Merge(const std::vector<std::u32string>& left, const std::vector<std::u32string>& right);

class Rule;

enum class NodeKind : int
{
    package, properties, components, component, directory, file, rules, rule, environment, environmentVariable, 
    pathDirectory, links, linkDirectories, linkDirectory, shortcuts, shortcut,
    engineVariables, engineVariable, content, contentDirectory, contentFile, dirsAndFiles
};

enum class MenuItemsKind : int
{
    none = 0, newMenuItems = 1 << 1, allMenuItems = 1 << 2, treeView = 1 << 3
};

inline MenuItemsKind operator|(MenuItemsKind left, MenuItemsKind right)
{
    return MenuItemsKind(int(left) | int(right));
}

inline MenuItemsKind operator&(MenuItemsKind left, MenuItemsKind right)
{
    return MenuItemsKind(int(left) & int(right));
}

inline MenuItemsKind operator~(MenuItemsKind kind)
{
    return MenuItemsKind(~int(kind));
}

class Package;
class PackageContentView;
class MainWindow;

class Node
{
public:
    Node(NodeKind kind_, const std::string& name_);
    virtual ~Node();
    NodeKind Kind() const { return kind; }
    const std::string& Name() const { return name; }
    void SetName(const std::string& name_);
    virtual std::string PathComponentName() const { return Name(); }
    virtual std::string ViewName() const { return Name(); }
    Node* Parent() const { return parent; }
    void SetParent(Node* parent_) { parent = parent_; }
    MainWindow* GetMainWindow();
    wing::TreeViewNode* GetTreeViewNode() const { return treeViewNode; }
    void SetTreeViewNode(wing::TreeViewNode* treeViewNode_) { treeViewNode = treeViewNode_; }
    void Explore();
    void ResetDirectoryPath();
    void ViewContent();
    void SetCurrentPathNode();
    void Open();
    void OpenAndExpand();
    void Select();
    virtual bool Leaf() const { return false; }
    virtual void ExecuteDefaultAction();
    virtual Package* GetPackage() const;
    virtual wing::Control* CreateView(wing::ImageList* imageList);
    virtual std::string ImageName() const;
    virtual bool CanDisable() const { return false; }
    virtual bool IsDisabled() const { return false; }
    virtual void Enable() {}
    virtual void Disable() {}
    virtual void SetData(wing::ListViewItem* item, wing::ImageList* imageList);
    virtual int Count() const;
    virtual int IndexOf(const Node* child) const;
    virtual Node* GetNode(int index) const;
    virtual bool HasNode(const std::string& name) const;
    virtual void AddNode(Node* node);
    virtual void AddNew(NodeKind kind);
    virtual void AddRule(Rule* rule);
    virtual Node* GetRuleContainerNode() { return nullptr; }
    virtual void Edit();
    virtual std::unique_ptr<Node> RemoveChild(int index);
    virtual void InsertBefore(int index, std::unique_ptr<Node>&& child);
    virtual void InsertAfter(int index, std::unique_ptr<Node>&& child);
    virtual bool CanOpen() const { return true; }
    virtual bool CanAdd() const { return false; }
    virtual bool CanEdit() const { return false; }
    virtual bool CanMoveUp(const Node* child) const;
    virtual bool CanMoveDown(const Node* child) const;
    virtual bool CanExclude() const { return false; }
    virtual bool CanInclude() const { return false; }
    virtual bool CanSave() const { return false; }
    virtual bool CanBuild() const { return false; }
    virtual bool CanClose() const { return false; }
    virtual bool CanClear() const { return false; }
    virtual void Save();
    virtual void Build();
    virtual void Close();
    virtual void Clear();
    virtual std::string DirectoryPath() const;
    virtual int RuleCount() const { return 0; }
    virtual Rule* GetRule(int index) const { return nullptr; }
    virtual Rule* GetRule(const std::string& name) const { return nullptr; }
    bool CanRemove() const;
    bool CanMoveUp() const;
    bool CanMoveDown() const;
    void Remove();
    void MoveUp();
    void MoveDown();
    void AddMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions, MenuItemsKind menuItemsKind);
    virtual void AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions);
    bool IncludeDirectory(const std::u32string& directoryName) const;
    bool IncludeFile(const std::u32string& fileName) const;
    wing::Control* CreateContentView(wing::ImageList* imageList);
private:
    NodeKind kind;
    std::string name;
    Node* parent;
    wing::TreeViewNode* treeViewNode;
    wing::ListViewItem* listViewItem;
};

} // package_editor

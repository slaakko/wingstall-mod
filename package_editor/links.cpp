// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.links;

import package_editor.link_directory_dialog;
import package_editor.shortcut_dialog;
import package_editor.action;
import package_editor.error;
import package_editor.main_window;
import package_editor.configuration;
import xpath;
import util;

namespace package_editor {

Links::Links() : Node(NodeKind::links, "Links")
{
    linkDirectories.reset(new LinkDirectories());
    linkDirectories->SetParent(this);
    shortcuts.reset(new Shortcuts());
    shortcuts->SetParent(this);
}

Links::Links(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::links, "Links")
{
    linkDirectories.reset(new LinkDirectories());
    linkDirectories->SetParent(this);
    shortcuts.reset(new Shortcuts());
    shortcuts->SetParent(this);
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
                    LinkDirectory* linkDirectory = new LinkDirectory(packageXMLFilePath, element);
                    linkDirectories->AddLinkDirectory(linkDirectory);
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
                    Shortcut* shortcut = new Shortcut(packageXMLFilePath, element);
                    shortcuts->AddShortcut(shortcut);
                }
            }
        }
    }
}

xml::Element* Links::ToXml() const
{
    xml::Element* element = xml::MakeElement("links");
    linkDirectories->AddElements(element);
    shortcuts->AddElements(element);
    return element;
}

wing::TreeViewNode* Links::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode("Links");
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("links.bitmap"));
    }
    node->AddChild(linkDirectories->ToTreeViewNode(view));
    node->AddChild(shortcuts->ToTreeViewNode(view));
    return node;
}

wing::Control* Links::CreateView(wing::ImageList* imageList)
{
    std::unique_ptr<wing::ListView> listView(new wing::ListView(wing::ListViewCreateParams().Defaults().SetDock(wing::Dock::fill)));
    listView->SetData(this);
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        mainWindow->AddListViewEventHandlers(listView.get());
        listView->ColumnWidthChanged().AddHandler(mainWindow, &MainWindow::ListViewColumnWidthChanged);
    }
    listView->SetDoubleBuffered();
    listView->SetImageList(imageList);
    int nameColumnWidthValue = 200;
    View& view = GetConfiguredViewSettings().GetView(ViewName());
    ColumnWidth& nameColumnWidth = view.GetColumnWidth("Name");
    if (nameColumnWidth.IsDefined())
    {
        nameColumnWidthValue = nameColumnWidth.Get();
    }
    else
    {
        nameColumnWidth.Set(nameColumnWidthValue);
    }
    listView->AddColumn("Name", nameColumnWidthValue);
    wing::ListViewItem* linkDirectoriesItem = listView->AddItem();
    linkDirectories->SetData(linkDirectoriesItem, imageList);
    wing::ListViewItem* shortcutsItem = listView->AddItem();
    shortcuts->SetData(shortcutsItem, imageList);
    return listView.release();

}

LinkDirectories::LinkDirectories() : Node(NodeKind::linkDirectories, "Link Directories")
{
}

void LinkDirectories::AddElements(xml::Element* parentElement)
{
    for (const auto& linkDirectory : linkDirectories)
    {
        parentElement->AppendChild(linkDirectory->ToXml());
    }
}

void LinkDirectories::AddLinkDirectory(LinkDirectory* linkDirectory)
{
    linkDirectory->SetParent(this);
    linkDirectories.push_back(std::unique_ptr<LinkDirectory>(linkDirectory));
}

wing::TreeViewNode* LinkDirectories::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode("Link Directories");
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("linked.folder.closed.bitmap"));
    }
    for (const auto& linkDirectory : linkDirectories)
    {
        node->AddChild(linkDirectory->ToTreeViewNode(view));
    }
    return node;
}

wing::Control* LinkDirectories::CreateView(wing::ImageList* imageList)
{
    std::unique_ptr<wing::ListView> listView(new wing::ListView(wing::ListViewCreateParams().Defaults().SetDock(wing::Dock::fill)));
    listView->SetData(this);
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        mainWindow->AddListViewEventHandlers(listView.get());
        listView->ColumnWidthChanged().AddHandler(mainWindow, &MainWindow::ListViewColumnWidthChanged);
    }
    listView->SetDoubleBuffered();
    listView->SetImageList(imageList);

    int pathColumnWidthValue = 400;
    View& view = GetConfiguredViewSettings().GetView(ViewName());
    ColumnWidth& pathColumnWidth = view.GetColumnWidth("Path");
    if (pathColumnWidth.IsDefined())
    {
        pathColumnWidthValue = pathColumnWidth.Get();
    }
    else
    {
        pathColumnWidth.Set(pathColumnWidthValue);
    }
    listView->AddColumn("Path", pathColumnWidthValue);
    for (const auto& linkDirectory : linkDirectories)
    {
        wing::ListViewItem* item = listView->AddItem();
        linkDirectory->SetData(item, imageList);
    }
    return listView.release();
}

int LinkDirectories::Count() const
{
    return linkDirectories.size();
}

int LinkDirectories::IndexOf(const Node* child) const
{
    if (child->Kind() == NodeKind::linkDirectory)
    {
        const LinkDirectory* linkDirectory = static_cast<const LinkDirectory*>(child);
        int n = linkDirectories.size();
        for (int i = 0; i < n; ++i)
        {
            if (linkDirectories[i].get() == linkDirectory)
            {
                return i;
            }
        }
    }
    return -1;
}

Node* LinkDirectories::GetNode(int index) const
{
    if (index >= 0 && index < Count())
    {
        return linkDirectories[index].get();
    }
    return nullptr;
}

bool LinkDirectories::HasNode(const std::string& name) const
{
    for (const auto& linkDirectory : linkDirectories)
    {
        if (linkDirectory->Path() == name) return true;
    }
    return false;
}

std::unique_ptr<Node> LinkDirectories::RemoveChild(int index)
{
    if (index >= 0 && index < Count())
    {
        LinkDirectory* linkDirectory = linkDirectories[index].release();
        linkDirectories.erase(linkDirectories.begin() + index);
        return std::unique_ptr<Node>(linkDirectory);
    }
    return std::unique_ptr<Node>();
}

void LinkDirectories::InsertBefore(int index, std::unique_ptr<Node>&& child)
{
    if (child->Kind() == NodeKind::linkDirectory && index >= 0 && index < Count())
    {
        LinkDirectory* linkDirectory = static_cast<LinkDirectory*>(child.release());
        linkDirectories.insert(linkDirectories.begin() + index, std::unique_ptr<LinkDirectory>(linkDirectory));
    }
    else
    {
        child.reset();
    }
}

void LinkDirectories::InsertAfter(int index, std::unique_ptr<Node>&& child)
{
    if (child->Kind() == NodeKind::linkDirectory && index >= 0 && index < Count())
    {
        LinkDirectory* linkDirectory = static_cast<LinkDirectory*>(child.release());
        linkDirectories.insert(linkDirectories.begin() + index + 1, std::unique_ptr<LinkDirectory>(linkDirectory));
    }
    else
    {
        child.reset();
    }
}

void LinkDirectories::AddNew(NodeKind kind)
{
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        if (kind == NodeKind::linkDirectory)
        {
            LinkDirectoryDialog dialog("Add New Link Directory");
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                std::unique_ptr<LinkDirectory> linkDirectoryPtr(new LinkDirectory());
                LinkDirectory* linkDirectory = linkDirectoryPtr.get();
                dialog.GetData(linkDirectory);
                if (HasNode(linkDirectory->Path()))
                {
                    throw std::runtime_error("path not unique");
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                AddLinkDirectory(linkDirectoryPtr.release());
                wing::TreeViewNode* linkDirectoriesTreeViewNode = GetTreeViewNode();
                if (linkDirectoriesTreeViewNode)
                {
                    wing::TreeView* treeView = linkDirectoriesTreeViewNode->GetTreeView();
                    if (treeView)
                    {
                        wing::TreeViewNode* linkDirectoryTreeViewNode = linkDirectory->ToTreeViewNode(treeView);
                        linkDirectoriesTreeViewNode->AddChild(linkDirectoryTreeViewNode);
                        treeView->SetSelectedNode(linkDirectoryTreeViewNode);
                    }
                }
                OpenAndExpand();
            }
        }
    }
}

void LinkDirectories::AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions)
{
    std::unique_ptr<wing::MenuItem> addNewLinkDirectoryMenuItem(new wing::MenuItem("Add New Link Directory"));
    clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddAction(addNewLinkDirectoryMenuItem.get(), this, NodeKind::linkDirectory)));
    contextMenu->AddMenuItem(addNewLinkDirectoryMenuItem.release());
}

LinkDirectory::LinkDirectory() : Node(NodeKind::linkDirectory, "Link Directory")
{
}

LinkDirectory::LinkDirectory(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::linkDirectory, "Link Directory")
{
    std::string pathAttr = element->GetAttribute("path");
    if (!pathAttr.empty())
    {
        SetPath(pathAttr);
    }
    else
    {
        throw PackageXMLException("'linkDirectory' element has no 'path' attribute", packageXMLFilePath, element);
    }
}

xml::Element* LinkDirectory::ToXml() const
{
    xml::Element* element = xml::MakeElement("linkDirectory");
    element->SetAttribute("path", path);
    return element;
}

wing::TreeViewNode* LinkDirectory::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode(path);
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("linked.folder.closed.bitmap"));
    }
    return node;
}

void LinkDirectory::SetPath(const std::string& path_)
{
    path = path_;
}

void LinkDirectory::SetData(wing::ListViewItem* item, wing::ImageList* imageList)
{
    Node::SetData(item, imageList);
    item->SetColumnValue(0, path);
}

void LinkDirectory::Edit()
{
    LinkDirectoryDialog dialog("Edit Link Directory");
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        Node* parent = Parent();
        if (parent && parent->Kind() == NodeKind::linkDirectories)
        {
            LinkDirectories* linkDirectories = static_cast<LinkDirectories*>(parent);
            std::string prevValue = Path();
            dialog.SetData(this);
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                if (dialog.PathDirectoryValue() != prevValue)
                {
                    if (linkDirectories->HasNode(dialog.PathDirectoryValue()))
                    {
                        throw std::runtime_error("path not unique");
                    }
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                dialog.GetData(this);
                linkDirectories->OpenAndExpand();
                wing::TreeViewNode* linkDirectoryTreeViewNode = GetTreeViewNode();
                if (linkDirectoryTreeViewNode)
                {
                    linkDirectoryTreeViewNode->SetText(Path());
                }
            }
        }
    }
}

Shortcuts::Shortcuts() : Node(NodeKind::shortcuts, "Shortcuts")
{
}

void Shortcuts::AddElements(xml::Element* parentElement)
{
    for (const auto& shortcut : shortcuts)
    {
        parentElement->AppendChild(shortcut->ToXml());
    }
}

void Shortcuts::AddShortcut(Shortcut* shortcut)
{
    shortcut->SetParent(this);
    shortcuts.push_back(std::unique_ptr<Shortcut>(shortcut));
}

wing::TreeViewNode* Shortcuts::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode("Shortcuts");
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("shortcut.bitmap"));
    }
    for (const auto& shortcut : shortcuts)
    {
        node->AddChild(shortcut->ToTreeViewNode(view));
    }
    return node;
}

wing::Control* Shortcuts::CreateView(wing::ImageList* imageList)
{
    std::unique_ptr<wing::ListView> listView(new wing::ListView(wing::ListViewCreateParams().Defaults().SetDock(wing::Dock::fill)));
    listView->SetData(this);
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        mainWindow->AddListViewEventHandlers(listView.get());
        listView->ColumnWidthChanged().AddHandler(mainWindow, &MainWindow::ListViewColumnWidthChanged);
    }
    listView->SetDoubleBuffered();
    listView->SetImageList(imageList);
    int linkFilePathColumnWidthValue = 400;
    View& view = GetConfiguredViewSettings().GetView(ViewName());
    ColumnWidth& linkFilePathColumnWidth = view.GetColumnWidth("Link File Path");
    if (linkFilePathColumnWidth.IsDefined())
    {
        linkFilePathColumnWidthValue = linkFilePathColumnWidth.Get();
    }
    else
    {
        linkFilePathColumnWidth.Set(linkFilePathColumnWidthValue);
    }
    listView->AddColumn("Link File Path", linkFilePathColumnWidthValue);
    int pathColumnWidthValue = 400;
    ColumnWidth& pathColumnWidth = view.GetColumnWidth("Path");
    if (pathColumnWidth.IsDefined())
    {
        pathColumnWidthValue = pathColumnWidth.Get();
    }
    else
    {
        pathColumnWidth.Set(pathColumnWidthValue);
    }
    listView->AddColumn("Path", pathColumnWidthValue);
    for (const auto& shortcut : shortcuts)
    {
        wing::ListViewItem* item = listView->AddItem();
        shortcut->SetData(item, imageList);
    }
    return listView.release();
}

int Shortcuts::Count() const
{
    return shortcuts.size();
}

int Shortcuts::IndexOf(const Node* child) const
{
    if (child->Kind() == NodeKind::shortcut)
    {
        const Shortcut* shortcut = static_cast<const Shortcut*>(child);
        int n = shortcuts.size();
        for (int i = 0; i < n; ++i)
        {
            if (shortcuts[i].get() == shortcut)
            {
                return i;
            }
        }
    }
    return -1;
}

Node* Shortcuts::GetNode(int index) const
{
    if (index >= 0 && index < Count())
    {
        return shortcuts[index].get();
    }
    return nullptr;
}

bool Shortcuts::HasNode(const std::string& name) const
{
    for (const auto& shortcut : shortcuts)
    {
        if (shortcut->LinkFilePath() == name)
        {
            return true;
        }
    }
    return false;
}

std::unique_ptr<Node> Shortcuts::RemoveChild(int index)
{
    if (index >= 0 && index < Count())
    {
        Shortcut* shortcut = shortcuts[index].release();
        shortcuts.erase(shortcuts.begin() + index);
        return std::unique_ptr<Node>(shortcut);
    }
    return std::unique_ptr<Node>();
}

void Shortcuts::InsertBefore(int index, std::unique_ptr<Node>&& child)
{
    if (child->Kind() == NodeKind::shortcut && index >= 0 && index < Count())
    {
        Shortcut* shortcut = static_cast<Shortcut*>(child.release());
        shortcuts.insert(shortcuts.begin() + index, std::unique_ptr<Shortcut>(shortcut));
    }
    else
    {
        child.reset();
    }
}

void Shortcuts::InsertAfter(int index, std::unique_ptr<Node>&& child)
{
    if (child->Kind() == NodeKind::shortcut && index >= 0 && index < Count())
    {
        Shortcut* shortcut = static_cast<Shortcut*>(child.release());
        shortcuts.insert(shortcuts.begin() + index + 1, std::unique_ptr<Shortcut>(shortcut));
    }
    else
    {
        child.reset();
    }
}

void Shortcuts::AddNew(NodeKind kind)
{
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        if (kind == NodeKind::shortcut)
        {
            ShortcutDialog dialog("Add New Shortcut");
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                std::unique_ptr<Shortcut> shortcutPtr(new Shortcut());
                Shortcut* shortcut = shortcutPtr.get();
                dialog.GetData(shortcut);
                if (HasNode(shortcut->LinkFilePath()))
                {
                    throw std::runtime_error("link file path not unique");
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                AddShortcut(shortcutPtr.release());
                wing::TreeViewNode* shortcutsTreeViewNode = GetTreeViewNode();
                if (shortcutsTreeViewNode)
                {
                    wing::TreeView* treeView = shortcutsTreeViewNode->GetTreeView();
                    if (treeView)
                    {
                        wing::TreeViewNode* shortcutTreeViewNode = shortcut->ToTreeViewNode(treeView);
                        shortcutsTreeViewNode->AddChild(shortcutTreeViewNode);
                        treeView->SetSelectedNode(shortcutsTreeViewNode);
                    }
                }
                OpenAndExpand();
            }
        }
    }
}

void Shortcuts::AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions)
{
    std::unique_ptr<wing::MenuItem> addNewShortcutMenuItem(new wing::MenuItem("Add New Shortcut"));
    clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddAction(addNewShortcutMenuItem.get(), this, NodeKind::shortcut)));
    contextMenu->AddMenuItem(addNewShortcutMenuItem.release());
}

Shortcut::Shortcut() : Node(NodeKind::shortcut, "Shortcut"), iconIndex(0)
{
}

Shortcut::Shortcut(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::shortcut, "Shortcut"), iconIndex(0)
{
    std::string linkFilePathAttr = element->GetAttribute("linkFilePath");
    if (!linkFilePathAttr.empty())
    {
        linkFilePath = linkFilePathAttr;
    }
    else
    {
        throw PackageXMLException("'shortcut' element has no 'linkFilePath' attribute", packageXMLFilePath, element);
    }
    std::string pathAttr = element->GetAttribute("path");
    if (!pathAttr.empty())
    {
        path = pathAttr;
    }
    else
    {
        throw PackageXMLException("'shortcut' element has no 'path' attribute", packageXMLFilePath, element);
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
        iconFilePath = iconPathAttr;
    }
    std::string iconIndexAttr = element->GetAttribute("iconIndex");
    if (!iconIndexAttr.empty())
    {
        iconIndex = std::stoi(iconIndexAttr);
    }
}

xml::Element* Shortcut::ToXml() const
{
    xml::Element* element = xml::MakeElement("link");
    element->SetAttribute("linkFilePath", linkFilePath);
    element->SetAttribute("path", path);
    if (!arguments.empty())
    {
        element->SetAttribute("arguments", arguments);
    }
    if (!workingDirectory.empty())
    {
        element->SetAttribute("workingDirectory", workingDirectory);
    }
    if (!description.empty())
    {
        element->SetAttribute("description", description);
    }
    if (!iconFilePath.empty())
    {
        element->SetAttribute("iconPath", iconFilePath);
    }
    element->SetAttribute("iconIndex", std::to_string(iconIndex));
    return element;
}

wing::TreeViewNode* Shortcut::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode(linkFilePath);
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("shortcut.bitmap"));
    }
    return node;
}

void Shortcut::SetData(wing::ListViewItem* item, wing::ImageList* imageList)
{
    Node::SetData(item, imageList);
    item->SetColumnValue(0, linkFilePath);
    item->SetColumnValue(1, path);
}

void Shortcut::SetLinkFilePath(const std::string& linkFilePath_)
{
    linkFilePath = linkFilePath_;
}

void Shortcut::SetPath(const std::string& path_)
{
    path = path_;
}

void Shortcut::SetArguments(const std::string& arguments_)
{
    arguments = arguments_;
}

void Shortcut::SetWorkingDirectory(const std::string& workingDirectory_)
{
    workingDirectory = workingDirectory_;
}

void Shortcut::SetDescription(const std::string& description_)
{
    description = description_;
}

void Shortcut::SetIconFilePath(const std::string& iconFilePath_)
{
    iconFilePath = iconFilePath_;
}

void Shortcut::Edit()
{
    ShortcutDialog dialog("Edit Shortcut");
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        Node* parent = Parent();
        if (parent && parent->Kind() == NodeKind::shortcuts)
        {
            Shortcuts* shortcuts = static_cast<Shortcuts*>(parent);
            std::string prevValue = LinkFilePath();
            dialog.SetData(this);
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                if (dialog.LinkFilePath() != prevValue)
                {
                    if (shortcuts->HasNode(dialog.LinkFilePath()))
                    {
                        throw std::runtime_error("link file path not unique");
                    }
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                dialog.GetData(this);
                shortcuts->OpenAndExpand();
                wing::TreeViewNode* shortcutTreeViewNode = GetTreeViewNode();
                if (shortcutTreeViewNode)
                {
                    shortcutTreeViewNode->SetText(LinkFilePath());
                }
            }
        }
    }

}

} // package_editor

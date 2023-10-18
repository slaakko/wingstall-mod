// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.component;

import package_editor.error;
import package_editor.main_window;
import package_editor.action;
import package_editor.component_dialog;
import package_editor.add_directories_and_files_dialog;
import package_editor.configuration;
import xpath;
import util;
import std.filesystem;

namespace package_editor {

struct DirectoryByName
{
    bool operator()(const std::unique_ptr<Directory>& left, const std::unique_ptr<Directory>& right) const
    {
        std::u32string l = util::ToUtf32(left->Name());
        std::u32string r = util::ToUtf32(right->Name());
        return l < r;
    }
};

struct FileByName
{
    bool operator()(const std::unique_ptr<File>& left, const std::unique_ptr<File>& right) const
    {
        std::u32string l = util::ToUtf32(left->Name());
        std::u32string r = util::ToUtf32(right->Name());
        return l < r;
    }
};

Components::Components() : Node(NodeKind::components, "Components")
{
}

void Components::AddElements(xml::Element* packageElement)
{
    for (const auto& component : components)
    {
        packageElement->AppendChild(component->ToXml());
    }
}

void Components::AddComponent(Component* component)
{
    component->SetParent(this);
    components.push_back(std::unique_ptr<Component>(component));
}

wing::TreeViewNode* Components::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode("Components");
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("components.bitmap"));
    }
    for (const auto& component : components)
    {
        node->AddChild(component->ToTreeViewNode(view));
    }
    return node;
}

wing::Control* Components::CreateView(wing::ImageList* imageList)
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
    for (const auto& component : components)
    {
        wing::ListViewItem* item = listView->AddItem();
        component->SetData(item, imageList);
    }
    return listView.release();
}

int Components::Count() const
{
    return components.size();
}

int Components::IndexOf(const Node* child) const
{
    if (child->Kind() == NodeKind::component)
    {
        const Component* component = static_cast<const Component*>(child);
        int n = components.size();
        for (int i = 0; i < n; ++i)
        {
            if (components[i].get() == component)
            {
                return i;
            }
        }
    }
    return -1;
}

Node* Components::GetNode(int index) const
{
    if (index >= 0 && index < Count())
    {
        return components[index].get();
    }
    return nullptr;
}

bool Components::HasNode(const std::string& name) const
{
    for (const auto& component : components)
    {
        if (component->Name() == name) return true;
    }
    return false;
}

std::unique_ptr<Node> Components::RemoveChild(int index)
{
    if (index >= 0 && index < Count())
    {
        Component* component = components[index].release();
        components.erase(components.begin() + index);
        return std::unique_ptr<Node>(component);
    }
    return std::unique_ptr<Node>();
}

void Components::InsertBefore(int index, std::unique_ptr<Node>&& child)
{
    if (child->Kind() == NodeKind::component && index >= 0 && index < Count())
    {
        Component* component = static_cast<Component*>(child.release());
        components.insert(components.begin() + index, std::unique_ptr<Component>(component));
    }
    else
    {
        child.reset();
    }
}

void Components::InsertAfter(int index, std::unique_ptr<Node>&& child)
{
    if (child->Kind() == NodeKind::component && index >= 0 && index < Count())
    {
        Component* component = static_cast<Component*>(child.release());
        components.insert(components.begin() + index + 1, std::unique_ptr<Component>(component));
    }
    else
    {
        child.reset();
    }
}

void Components::AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions)
{
    std::unique_ptr<wing::MenuItem> addNewComponentMenuItem(new wing::MenuItem("Add New Component"));
    clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddAction(addNewComponentMenuItem.get(), this, NodeKind::component)));
    contextMenu->AddMenuItem(addNewComponentMenuItem.release());
}

Component* Components::GetDirectoryComponent(const std::u32string& directoryName) const
{
    auto it = directoryNameComponentMap.find(directoryName);
    if (it != directoryNameComponentMap.cend())
    {
        return it->second;
    }
    return nullptr;
}

Component* Components::GetFileComponent(const std::u32string& fileName) const
{
    auto it = fileNameComponentMap.find(fileName);
    if (it != fileNameComponentMap.cend())
    {
        return it->second;
    }
    return nullptr;
}

void Components::MapDirectoryComponent(const std::u32string& directoryName, Component* component)
{
    directoryNameComponentMap[directoryName] = component;
}

void Components::MapFileComponent(const std::u32string& fileName, Component* component)
{
    fileNameComponentMap[fileName] = component;
}

void Components::RemoveDirectoryName(const std::u32string& directoryName)
{
    directoryNameComponentMap.erase(directoryName);
}

void Components::RemoveFileName(const std::u32string& fileName)
{
    fileNameComponentMap.erase(fileName);
}

void Components::RemoveUnexistingDirectoriesAndFiles()
{
    for (const std::unique_ptr<Component>& component : components)
    {
        component->RemoveUnexistingDirectoriesAndFiles();
    }
}

void Components::MakeDisjoint()
{
    std::vector<std::u32string> directoryNames;
    std::vector<std::u32string> fileNames;
    for (const std::unique_ptr<Component>& component : components)
    {
        component->FilterDirectories(directoryNames);
        directoryNames = Merge(directoryNames, component->DirectoryNames());
        component->FilterFiles(fileNames);
        fileNames = Merge(fileNames, component->FileNames());
    }
}

void Components::MakeMaps()
{
    for (const std::unique_ptr<Component>& component : components)
    {
        std::vector<std::u32string> directoryNames = component->DirectoryNames();
        for (const std::u32string& directoryName : directoryNames)
        {
            directoryNameComponentMap[directoryName] = component.get();
        }
        std::vector<std::u32string> fileNames = component->FileNames();
        for (const std::u32string& fileName : fileNames)
        {
            fileNameComponentMap[fileName] = component.get();
        }
    }
}

void Components::AddNew(NodeKind kind)
{
    if (kind == NodeKind::component)
    {
        ComponentDialog dialog("Add New Component");
        MainWindow* mainWindow = GetMainWindow();
        if (mainWindow)
        {
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                if (HasNode(dialog.ComponentName()))
                {
                    throw std::runtime_error("name not unique");
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                Component* component = new Component(dialog.ComponentName());
                AddComponent(component);
                wing::TreeViewNode* componentsTreeViewNode = GetTreeViewNode();
                if (componentsTreeViewNode)
                {
                    wing::TreeView* treeView = componentsTreeViewNode->GetTreeView();
                    if (treeView)
                    {
                        wing::TreeViewNode* componentTreeViewNode = component->ToTreeViewNode(treeView);
                        componentsTreeViewNode->AddChild(componentTreeViewNode);
                        treeView->SetSelectedNode(componentsTreeViewNode);
                    }
                }
                OpenAndExpand();
            }
        }
    }
}

Component::Component(const std::string& name_) : Node(NodeKind::component, name_)
{
}

Component::Component(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::component, std::string())
{
    std::string nameAttr = element->GetAttribute("name");
    if (!nameAttr.empty())
    {
        SetName(nameAttr);
    }
    else
    {
        throw PackageXMLException("'component' element has no 'name' attribute", packageXMLFilePath, element);
    }
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
                    Directory* directory = new Directory(packageXMLFilePath, element);
                    AddDirectory(directory);
                }
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> fileObject = xpath::Evaluate("file", element);
    if (fileObject)
    {
        if (fileObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(fileObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    File* file = new File(packageXMLFilePath, element);
                    AddFile(file);
                }
            }
        }
    }
    Sort();
}

xml::Element* Component::ToXml() const
{
    xml::Element* element = xml::MakeElement("component");
    element->SetAttribute("name", Name());
    for (const auto& directory : directories)
    {
        element->AppendChild(directory->ToXml());
    }
    for (const auto& file : files)
    {
        element->AppendChild(file->ToXml());
    }
    return element;
}

wing::TreeViewNode* Component::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode(Name());
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("component.bitmap"));
    }
    for (const auto& directory : directories)
    {
        node->AddChild(directory->ToTreeViewNode(view));
    }
    for (const auto& file : files)
    {
        node->AddChild(file->ToTreeViewNode(view));
    }
    return node;
}

wing::Control* Component::CreateView(wing::ImageList* imageList)
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
    int nameColumnWidthValue = 400;
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
    for (const auto& directory : directories)
    {
        wing::ListViewItem* item = listView->AddItem();
        directory->SetData(item, imageList);
    }
    for (const auto& file : files)
    {
        wing::ListViewItem* item = listView->AddItem();
        file->SetData(item, imageList);
    }
    return listView.release();
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

int Component::Count() const
{
    return directories.size() + files.size();
}

int Component::IndexOf(const Node* child) const
{
    int nd = directories.size();
    if (child->Kind() == NodeKind::directory)
    {
        const Directory* directory = static_cast<const Directory*>(child);
        for (int i = 0; i < nd; ++i)
        {
            if (directories[i].get() == directory)
            {
                return i;
            }
        }
    }
    else if (child->Kind() == NodeKind::file)
    {
        const File* file = static_cast<const File*>(child);
        int n = files.size();
        for (int i = 0; i < n; ++i)
        {
            if (files[i].get() == file)
            {
                return nd + i;
            }
        }
    }
    return -1;
}

Node* Component::GetNode(int index) const
{
    int nd = directories.size();
    if (index >= 0 && index < nd)
    {
        return directories[index].get();
    }
    else if (index >= nd && index < Count())
    {
        return files[index - nd].get();
    }
    return nullptr;
}

std::unique_ptr<Node> Component::RemoveChild(int index)
{
    Node* parent = Parent();
    if (parent && parent->Kind() == NodeKind::components)
    {
        Components* components = static_cast<Components*>(parent);
        int nd = directories.size();
        if (index >= 0 && index < nd)
        {
            Directory* directory = directories[index].release();
            components->RemoveDirectoryName(util::ToUtf32(directory->Name()));
            directories.erase(directories.begin() + index);
            return std::unique_ptr<Node>(directory);
        }
        else if (index >= nd && index < Count())
        {
            File* file = files[index - nd].release();
            components->RemoveDirectoryName(util::ToUtf32(file->Name()));
            files.erase(files.begin() + (index - nd));
            return std::unique_ptr<Node>(file);
        }
    }
    return std::unique_ptr<Node>();
}

void Component::InsertBefore(int index, std::unique_ptr<Node>&& child)
{
    Node* parent = Parent();
    if (parent && parent->Kind() == NodeKind::components)
    {
        Components* components = static_cast<Components*>(parent);
        int nd = directories.size();
        if (child->Kind() == NodeKind::directory && index >= 0 && index < nd)
        {
            Directory* directory = static_cast<Directory*>(child.release());
            components->MapDirectoryComponent(util::ToUtf32(directory->Name()), this);
            directories.insert(directories.begin() + index, std::unique_ptr<Directory>(directory));
        }
        else if (child->Kind() == NodeKind::file && index >= nd && index < Count())
        {
            File* file = static_cast<File*>(child.release());
            components->MapFileComponent(util::ToUtf32(file->Name()), this);
            files.insert(files.begin() + index - nd, std::unique_ptr<File>(file));
        }
        else
        {
            child.reset();
        }
    }
    else
    {
        child.reset();
    }
}

void Component::InsertAfter(int index, std::unique_ptr<Node>&& child)
{
    Node* parent = Parent();
    if (parent && parent->Kind() == NodeKind::components)
    {
        Components* components = static_cast<Components*>(parent);
        int nd = directories.size();
        if (child->Kind() == NodeKind::directory && index >= 0 && index < nd)
        {
            Directory* directory = static_cast<Directory*>(child.release());
            components->MapDirectoryComponent(util::ToUtf32(directory->Name()), this);
            directories.insert(directories.begin() + index + 1, std::unique_ptr<Directory>(directory));
        }
        else if (child->Kind() == NodeKind::file && index >= nd && index < Count())
        {
            File* file = static_cast<File*>(child.release());
            components->MapFileComponent(util::ToUtf32(file->Name()), this);
            files.insert(files.begin() + index - nd + 1, std::unique_ptr<File>(file));
        }
        else
        {
            child.reset();
        }
    }
    else
    {
        child.reset();
    }
}

bool Component::CanMoveUp(const Node* child) const
{
    return false;
}

bool Component::CanMoveDown(const Node* child) const
{
    return false;
}

void Component::Edit()
{
    ComponentDialog dialog("Edit Component");
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        std::string prevName = Name();
        dialog.SetComponentName(Name());
        if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
        {
            if (dialog.ComponentName() != prevName)
            {
                if (HasNode(dialog.ComponentName()))
                {
                    throw std::runtime_error("name not unique");
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                SetName(dialog.ComponentName());
                Open();
                wing::TreeViewNode* componentTreeViewNode = GetTreeViewNode();
                if (componentTreeViewNode)
                {
                    componentTreeViewNode->SetText(Name());
                }
            }
        }
    }
}

void Component::AddNew(NodeKind kind)
{
    if (kind == NodeKind::content)
    {
        AddDirectoriesAndFilesDialog dialog(this);
        MainWindow* mainWindow = GetMainWindow();
        if (mainWindow)
        {
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                std::vector<std::u32string> selectedDirectories;
                std::vector<std::u32string> selectedFiles;
                dialog.GetSelectedDirectoriesAndFiles(selectedDirectories, selectedFiles);
                AddDirectoriesAndFiles(selectedDirectories, selectedFiles);
                Node* parent = Parent();
                if (parent && parent->Kind() == NodeKind::components)
                {
                    Components* components = static_cast<Components*>(parent);
                    wing::TreeViewNode* componentsTreeViewNode = parent->GetTreeViewNode();
                    if (componentsTreeViewNode)
                    {
                        wing::TreeView* view = componentsTreeViewNode->GetTreeView();
                        if (view)
                        {
                            componentsTreeViewNode->RemoveChildren();
                            for (const auto& component : components->GetComponents())
                            {
                                componentsTreeViewNode->AddChild(component->ToTreeViewNode(view));
                            }
                        }
                    }
                }
                OpenAndExpand();
            }
        }
    }
}

void Component::AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions)
{
    std::unique_ptr<wing::MenuItem> addMenuItem(new wing::MenuItem("Add Directories and Files"));
    clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddAction(addMenuItem.get(), this, NodeKind::content)));
    contextMenu->AddMenuItem(addMenuItem.release());
}

std::vector<std::u32string> Component::DirectoryNames() const
{
    std::vector<std::u32string> result;
    for (const std::unique_ptr<Directory>& directory : directories)
    {
        result.push_back(util::ToUtf32(directory->Name()));
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<std::u32string> Component::FileNames() const
{
    std::vector<std::u32string> result;
    for (const std::unique_ptr<File>& file : files)
    {
        result.push_back(util::ToUtf32(file->Name()));
    }
    std::sort(result.begin(), result.end());
    return result;
}

void Component::FilterDirectories(const std::vector<std::u32string>& exclude)
{
    Node* parent = Parent();
    if (parent && parent->Kind() == NodeKind::components)
    {
        Components* components = static_cast<Components*>(parent);
        std::vector<std::unique_ptr<Directory>> result;
        for (std::unique_ptr<Directory>& directory : directories)
        {
            std::u32string directoryName = util::ToUtf32(directory->Name());
            if (!std::binary_search(exclude.begin(), exclude.end(), directoryName))
            {
                result.push_back(std::unique_ptr<Directory>(directory.release()));
            }
            else
            {
                components->RemoveDirectoryName(directoryName);
            }
        }
        std::swap(directories, result);
    }
}

void Component::FilterFiles(const std::vector<std::u32string>& exclude)
{
    Node* parent = Parent();
    if (parent && parent->Kind() == NodeKind::components)
    {
        Components* components = static_cast<Components*>(parent);
        std::vector<std::unique_ptr<File>> result;
        for (std::unique_ptr<File>& file : files)
        {
            std::u32string fileName = util::ToUtf32(file->Name());
            if (!std::binary_search(exclude.begin(), exclude.end(), fileName))
            {
                result.push_back(std::unique_ptr<File>(file.release()));
            }
            else
            {
                components->RemoveFileName(fileName);
            }
        }
        std::swap(files, result);
    }
}

void Component::Map(const std::vector<std::u32string>& directoryNames, const std::vector<std::u32string>& fileNames)
{
    Node* parent = Parent();
    if (parent && parent->Kind() == NodeKind::components)
    {
        Components* components = static_cast<Components*>(parent);
        for (const std::u32string& directoryName : directoryNames)
        {
            components->MapDirectoryComponent(directoryName, this);
        }
        for (const std::u32string& fileName : fileNames)
        {
            components->MapFileComponent(fileName, this);
        }
    }
}

void Component::Sort()
{
    std::sort(directories.begin(), directories.end(), DirectoryByName());
    std::sort(files.begin(), files.end(), FileByName());
}

void Component::RemoveUnexistingDirectoriesAndFiles()
{
    std::vector<std::u32string> directoriesToRemove;
    std::vector<std::u32string> filesToRemove;
    Package* package = GetPackage();
    if (package)
    {
        std::string sourceRootDir = package->GetProperties()->SourceRootDir();
        for (const auto& directory : directories)
        {
            std::filesystem::path p(util::MakeNativeFileSystemPath(util::Path::Combine(sourceRootDir, directory->Name())));
            if (!std::filesystem::exists(p))
            {
                directoriesToRemove.push_back(util::ToUtf32(directory->Name()));
            }
        }
        for (const auto& file : files)
        {
            std::filesystem::path p(util::MakeNativeFileSystemPath(util::Path::Combine(sourceRootDir, file->Name())));
            if (!std::filesystem::exists(p))
            {
                filesToRemove.push_back(util::ToUtf32(file->Name()));
            }
        }
        FilterDirectories(directoriesToRemove);
        FilterFiles(filesToRemove);
    }
}

void Component::AddDirectoriesAndFiles(const std::vector<std::u32string>& directoryNames, const std::vector<std::u32string>& fileNames)
{
    for (const std::u32string& directoryName : directoryNames)
    {
        AddDirectory(new Directory(util::ToUtf8(directoryName)));
    }
    for (const std::u32string& fileName : fileNames)
    {
        AddFile(new File(util::ToUtf8(fileName)));
    }
    Sort();
    Node* parent = Parent();
    if (parent && parent->Kind() == NodeKind::components)
    {
        std::vector<std::u32string> allDirectoryNames = DirectoryNames();
        std::vector<std::u32string> allFileNames = FileNames();
        Components* components = static_cast<Components*>(parent);
        int n = components->GetComponents().size();
        for (int i = 0; i < n; ++i)
        {
            Component* component = components->GetComponents()[i].get();
            if (component != this)
            {
                component->FilterDirectories(allDirectoryNames);
                component->FilterFiles(allFileNames);
            }
        }
        Map(allDirectoryNames, allFileNames);
    }
}

} // package_editor

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.environment;

import package_editor.error;
import package_editor.main_window;
import package_editor.action;
import package_editor.environment_variable_dialog;
import package_editor.path_directory_dialog;
import package_editor.configuration;
import xpath;
import util;

namespace package_editor {

Environment::Environment() : Node(NodeKind::environment, "Environment")
{
}

Environment::Environment(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::environment, "Environment")
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
                    EnvironmentVariable* environmentVariable = new EnvironmentVariable(packageXMLFilePath, element);
                    AddEnvironentVariable(environmentVariable);
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
                    PathDirectory* pathDirectory = new PathDirectory(packageXMLFilePath, element);
                    AddPathDirectory(pathDirectory);
                }
            }
        }
    }
}

xml::Element* Environment::ToXml() const
{
    xml::Element* element = xml::MakeElement("environment");
    for (const auto& environmentVariable : environmentVariables)
    {
        element->AppendChild(environmentVariable->ToXml());
    }
    for (const auto& pathDirectory : pathDirectories)
    {
        element->AppendChild(pathDirectory->ToXml());
    }
    return element;
}

wing::TreeViewNode* Environment::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode("Environment");
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("environment.bitmap"));
    }
    for (const auto& environmentVariable : environmentVariables)
    {
        node->AddChild(environmentVariable->ToTreeViewNode(view));
    }
    for (const auto& pathDirectory : pathDirectories)
    {
        node->AddChild(pathDirectory->ToTreeViewNode(view));
    }
    return node;
}

wing::Control* Environment::CreateView(wing::ImageList* imageList)
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
    int valueColumnWidthValue = 400;
    ColumnWidth& valueColumnWidth = view.GetColumnWidth("Value");
    if (valueColumnWidth.IsDefined())
    {
        valueColumnWidthValue = valueColumnWidth.Get();
    }
    else
    {
        valueColumnWidth.Set(valueColumnWidthValue);
    }
    listView->AddColumn("Value", valueColumnWidthValue);
    for (const auto& environmentVariable : environmentVariables)
    {
        wing::ListViewItem* item = listView->AddItem();
        environmentVariable->SetData(item, imageList);
    }
    for (const auto& pathDirectory : pathDirectories)
    {
        wing::ListViewItem* item = listView->AddItem();
        pathDirectory->SetData(item, imageList);
    }
    return listView.release();
}

void Environment::AddEnvironentVariable(EnvironmentVariable* environmentVariable)
{
    environmentVariable->SetParent(this);
    environmentVariables.push_back(std::unique_ptr<EnvironmentVariable>(environmentVariable));
}

void Environment::AddPathDirectory(PathDirectory* pathDirectory)
{
    pathDirectory->SetParent(this);
    pathDirectories.push_back(std::unique_ptr<PathDirectory>(pathDirectory));
}

int Environment::Count() const
{
    return environmentVariables.size() + pathDirectories.size();
}

int Environment::IndexOf(const Node* child) const
{
    int ne = environmentVariables.size();
    if (child->Kind() == NodeKind::environmentVariable)
    {
        const EnvironmentVariable* environmentVariable = static_cast<const EnvironmentVariable*>(child);
        for (int i = 0; i < ne; ++i)
        {
            if (environmentVariables[i].get() == environmentVariable)
            {
                return i;
            }
        }
    }
    else if (child->Kind() == NodeKind::pathDirectory)
    {
        const PathDirectory* pathDirectory = static_cast<const PathDirectory*>(child);
        int n = pathDirectories.size();
        for (int i = 0; i < n; ++i)
        {
            if (pathDirectories[i].get() == pathDirectory)
            {
                return ne + i;
            }
        }
    }
    return -1;
}

Node* Environment::GetNode(int index) const
{
    int ne = environmentVariables.size();
    if (index >= 0 && index < ne)
    {
        return environmentVariables[index].get();
    }
    else if (index >= ne && index < Count())
    {
        return pathDirectories[index - ne].get();
    }
    return nullptr;
}

std::unique_ptr<Node> Environment::RemoveChild(int index)
{
    int ne = environmentVariables.size();
    if (index >= 0 && index < ne)
    {
        EnvironmentVariable* environmentVariable = environmentVariables[index].release();
        environmentVariables.erase(environmentVariables.begin() + index);
        return std::unique_ptr<Node>(environmentVariable);
    }
    else if (index >= ne && index < Count())
    {
        PathDirectory* pathDirectory = pathDirectories[index - ne].release();
        pathDirectories.erase(pathDirectories.begin() + (index - ne));
        return std::unique_ptr<Node>(pathDirectory);
    }
    return std::unique_ptr<Node>();
}

void Environment::InsertBefore(int index, std::unique_ptr<Node>&& child)
{
    int ne = environmentVariables.size();
    if (child->Kind() == NodeKind::environmentVariable && index >= 0 && index < ne)
    {
        EnvironmentVariable* environmentVariable = static_cast<EnvironmentVariable*>(child.release());
        environmentVariables.insert(environmentVariables.begin() + index, std::unique_ptr<EnvironmentVariable>(environmentVariable));
    }
    else if (child->Kind() == NodeKind::pathDirectory && index >= ne && index < Count())
    {
        PathDirectory* pathDirectory = static_cast<PathDirectory*>(child.release());
        pathDirectories.insert(pathDirectories.begin() + index - ne, std::unique_ptr<PathDirectory>(pathDirectory));
    }
    else
    {
        child.reset();
    }
}

void Environment::InsertAfter(int index, std::unique_ptr<Node>&& child)
{
    int ne = environmentVariables.size();
    if (child->Kind() == NodeKind::environmentVariable && index >= 0 && index < ne)
    {
        EnvironmentVariable* environmentVariable = static_cast<EnvironmentVariable*>(child.release());
        environmentVariables.insert(environmentVariables.begin() + index + 1, std::unique_ptr<EnvironmentVariable>(environmentVariable));
    }
    else if (child->Kind() == NodeKind::pathDirectory && index >= ne && index < Count())
    {
        PathDirectory* pathDirectory = static_cast<PathDirectory*>(child.release());
        pathDirectories.insert(pathDirectories.begin() + index - ne + 1, std::unique_ptr<PathDirectory>(pathDirectory));
    }
    else
    {
        child.reset();
    }
}

bool Environment::CanMoveUp(const Node* child) const
{
    int ne = environmentVariables.size();
    if (child->Kind() == NodeKind::environmentVariable)
    {
        int index = IndexOf(child);
        return index > 0 && index < ne;
    }
    else if (child->Kind() == NodeKind::pathDirectory)
    {
        int index = IndexOf(child);
        return index - ne > 0 && index - ne < pathDirectories.size();
    }
    return false;
}

bool Environment::CanMoveDown(const Node* child) const
{
    int ne = environmentVariables.size();
    if (child->Kind() == NodeKind::environmentVariable)
    {
        int index = IndexOf(child);
        return index >= 0 && index < ne - 1;
    }
    else if (child->Kind() == NodeKind::pathDirectory)
    {
        int index = IndexOf(child);
        return index - ne >= 0 && index - ne < pathDirectories.size() - 1;
    }
    return false;
}

bool Environment::HasEnvironmentVariable(const std::string& name) const
{
    for (const auto& environmentVariable : environmentVariables)
    {
        if (environmentVariable->Name() == name) return true;
    }
    return false;
}

bool Environment::HasPathDirectory(const std::string& value) const
{
    for (const auto& pathDirectory : pathDirectories)
    {
        if (pathDirectory->Value() == value) return true;
    }
    return false;
}

void Environment::AddNew(NodeKind kind)
{
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        if (kind == NodeKind::environmentVariable)
        {
            EnvironmentVariableDialog dialog("Add New Environment Variable");
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                std::unique_ptr<EnvironmentVariable> environmentVariablePtr(new EnvironmentVariable());
                EnvironmentVariable* environmentVariable = environmentVariablePtr.get();
                dialog.GetData(environmentVariable);
                if (HasEnvironmentVariable(environmentVariable->Name()))
                {
                    throw std::runtime_error("name not unique");
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                AddEnvironentVariable(environmentVariablePtr.release());
                wing::TreeViewNode* environmentTreeViewNode = GetTreeViewNode();
                if (environmentTreeViewNode)
                {
                    wing::TreeView* treeView = environmentTreeViewNode->GetTreeView();
                    if (treeView)
                    {
                        wing::TreeViewNode* environmentVariableTreeViewNode = environmentVariable->ToTreeViewNode(treeView);
                        environmentTreeViewNode->AddChild(environmentVariableTreeViewNode);
                        treeView->SetSelectedNode(environmentTreeViewNode);
                    }
                }
                OpenAndExpand();
            }
        }
        else if (kind == NodeKind::pathDirectory)
        {
            PathDirectoryDialog dialog("Add New PATH Directory");
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                std::unique_ptr<PathDirectory> pathDirectoryPtr(new PathDirectory());
                PathDirectory* pathDirectory = pathDirectoryPtr.get();
                dialog.GetData(pathDirectory);
                if (HasPathDirectory(pathDirectory->Value()))
                {
                    throw std::runtime_error("path not unique");
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                AddPathDirectory(pathDirectoryPtr.release());
                wing::TreeViewNode* environmentTreeViewNode = GetTreeViewNode();
                if (environmentTreeViewNode)
                {
                    wing::TreeView* treeView = environmentTreeViewNode->GetTreeView();
                    if (treeView)
                    {
                        wing::TreeViewNode* pathDirectoryTreeViewNode = pathDirectory->ToTreeViewNode(treeView);
                        environmentTreeViewNode->AddChild(pathDirectoryTreeViewNode);
                        treeView->SetSelectedNode(environmentTreeViewNode);
                    }
                }
                OpenAndExpand();
            }
        }
    }
}

void Environment::AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions)
{
    std::unique_ptr<wing::MenuItem> addNewEnvironmentVariableMenuItem(new wing::MenuItem("Add New Environment Variable"));
    clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddAction(addNewEnvironmentVariableMenuItem.get(), this, NodeKind::environmentVariable)));
    contextMenu->AddMenuItem(addNewEnvironmentVariableMenuItem.release());

    std::unique_ptr<wing::MenuItem> addNewPathDirectoryMenuItem(new wing::MenuItem("Add New PATH Directory"));
    clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddAction(addNewPathDirectoryMenuItem.get(), this, NodeKind::pathDirectory)));
    contextMenu->AddMenuItem(addNewPathDirectoryMenuItem.release());
}

EnvironmentVariable::EnvironmentVariable() : Node(NodeKind::environmentVariable, std::string())
{
}

EnvironmentVariable::EnvironmentVariable(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::environmentVariable, std::string())
{
    std::string nameAttr = element->GetAttribute("name");
    if (!nameAttr.empty())
    {
        SetName(nameAttr);
    }
    else
    {
        throw PackageXMLException("'variable' element has no 'name' attribute", packageXMLFilePath, element);
    }
    std::string valueAttr = element->GetAttribute("value");
    if (!valueAttr.empty())
    {
        SetValue(valueAttr);
    }
    else
    {
        throw PackageXMLException("'variable' element has no 'value' attribute", packageXMLFilePath, element);
    }
}

xml::Element* EnvironmentVariable::ToXml() const
{
    xml::Element* element = xml::MakeElement("variable");
    element->SetAttribute("name", Name());
    element->SetAttribute("value", Value());
    return element;
}

void EnvironmentVariable::SetValue(const std::string& value_)
{
    value = value_;
}

void EnvironmentVariable::SetData(wing::ListViewItem* item, wing::ImageList* imageList)
{
    Node::SetData(item, imageList);
    item->SetColumnValue(1, value);
}

wing::TreeViewNode* EnvironmentVariable::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode(Name() + "=" + value);
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("environment.var.bitmap"));
    }
    return node;
}

void EnvironmentVariable::Edit()
{
    EnvironmentVariableDialog dialog("Edit Environment Variable");
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        Node* parent = Parent();
        if (parent && parent->Kind() == NodeKind::environment)
        {
            Environment* environment = static_cast<Environment*>(parent);
            std::string prevName = Name();
            dialog.SetData(this);
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                if (dialog.EnvironmentVariableName() != prevName)
                {
                    if (environment->HasEnvironmentVariable(dialog.EnvironmentVariableName()))
                    {
                        throw std::runtime_error("name not unique");
                    }
                }
                Package* package = GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                dialog.GetData(this);
                environment->OpenAndExpand();
                wing::TreeViewNode* environmentVariableTreeViewNode = GetTreeViewNode();
                if (environmentVariableTreeViewNode)
                {
                    environmentVariableTreeViewNode->SetText(Name() + "=" + Value());
                }
            }
        }
    }
}

PathDirectory::PathDirectory() : Node(NodeKind::pathDirectory, std::string())
{
}

PathDirectory::PathDirectory(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::pathDirectory, "PATH Directory")
{
    std::string valueAttr = element->GetAttribute("value");
    if (!valueAttr.empty())
    {
        SetValue(valueAttr);
    }
    else
    {
        throw PackageXMLException("'pathDirectory' element has no 'value' attribute", packageXMLFilePath, element);
    }
}

xml::Element* PathDirectory::ToXml() const
{
    xml::Element* element = xml::MakeElement("pathDirectory");
    element->SetAttribute("value", Value());
    return element;
}

void PathDirectory::SetValue(const std::string& value_)
{
    value = value_;
}

wing::TreeViewNode* PathDirectory::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode(value);
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("path.directory.bitmap"));
    }
    return node;
}

void PathDirectory::SetData(wing::ListViewItem* item, wing::ImageList* imageList)
{
    Node::SetData(item, imageList);
    item->SetColumnValue(1, value);
}

void PathDirectory::Edit()
{
    PathDirectoryDialog dialog("Edit PATH Directory");
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        Node* parent = Parent();
        if (parent && parent->Kind() == NodeKind::environment)
        {
            Environment* environment = static_cast<Environment*>(parent);
            std::string prevValue = Value();
            dialog.SetData(this);
            if (dialog.ShowDialog(*mainWindow) == wing::DialogResult::ok)
            {
                if (dialog.PathDirectoryValue() != prevValue)
                {
                    if (environment->HasPathDirectory(dialog.PathDirectoryValue()))
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
                environment->OpenAndExpand();
                wing::TreeViewNode* pathDirectoryTreeViewNode = GetTreeViewNode();
                if (pathDirectoryTreeViewNode)
                {
                    pathDirectoryTreeViewNode->SetText(Value());
                }
            }
        }
    }
}

} // package_editor

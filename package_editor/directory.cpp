// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.directory;

import package_editor.error;
import package_editor.main_window;
import package_editor.configuration;
import xpath;
import util;

namespace package_editor {

Directory::Directory(const std::string& name_) : Node(NodeKind::directory, name_)
{
    rules.reset(new Rules());
    rules->SetParent(this);
    content.reset(new Content());
    content->SetParent(this);
}

Directory::Directory(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::directory, std::string())
{
    rules.reset(new Rules());
    rules->SetParent(this);
    content.reset(new Content());
    content->SetParent(this);
    std::string nameAttr = element->GetAttribute("name");
    if (!nameAttr.empty())
    {
        SetName(nameAttr);
    }
    else
    {
        throw PackageXMLException("'directory' element has no 'name' attribute", packageXMLFilePath, element);
    }
    std::unique_ptr<xpath::XPathObject> ruleObject = xpath::Evaluate("*", element);
    if (ruleObject)
    {
        if (ruleObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(ruleObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    Rule* rule = new Rule(packageXMLFilePath, element);
                    rules->AddRule(rule);
                }
            }
        }
    }
}

xml::Element* Directory::ToXml() const
{
    xml::Element* element = xml::MakeElement("directory");
    element->SetAttribute("name", Name());
    rules->AddElements(element);
    return element;
}

wing::TreeViewNode* Directory::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode(Name());
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("folder.closed.bitmap"));
        node->SetExpandedImageIndex(imageList->GetImageIndex("folder.opened.bitmap"));
    }
    node->AddChild(rules->ToTreeViewNode(view));
    node->AddChild(content->ToTreeViewNode(view));
    return node;
}

wing::Control* Directory::CreateView(wing::ImageList* imageList)
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
    wing::ListViewItem* rulesItem = listView->AddItem();
    rules->SetData(rulesItem, imageList);
    wing::ListViewItem* contentItem = listView->AddItem();
    content->SetData(contentItem, imageList);
    return listView.release();
}

std::string Directory::DirectoryPath() const
{
    std::string sourceRootDir = "C:/";
    Package* package = GetPackage();
    if (package && package->GetProperties())
    {
        sourceRootDir = package->GetProperties()->SourceRootDir();
    }
    return util::GetFullPath(util::Path::Combine(sourceRootDir, Name()));
}

int Directory::RuleCount() const
{
    return rules->RuleCount();
}

Rule* Directory::GetRule(int index) const
{
    return rules->GetRule(index);
}

Rule* Directory::GetRule(const std::string& name) const
{
    return rules->GetRule(name);
}

Node* Directory::GetRuleContainerNode()
{
    return rules.get();
}

} // package_editor

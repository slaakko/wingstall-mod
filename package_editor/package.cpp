// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.package;

import package_editor.error;
import package_editor.main_window;
import package_editor.properties_view;
import package_editor.configuration;
import xpath;
import std.filesystem;

namespace package_editor {

Properties::Properties() : Node(NodeKind::properties, "Properties"), compression(wing_package::Compression::none), includeUninstaller(true), id(util::nil_uuid())
{
}

void Properties::SetAttributes(xml::Element* packageElement)
{
    packageElement->SetAttribute("sourceRootDir", sourceRootDir);
    packageElement->SetAttribute("targetRootDir", targetRootDir);
    packageElement->SetAttribute("appName", appName);
    if (!publisher.empty())
    {
        packageElement->SetAttribute("publisher", publisher);
    }
    if (!version.empty())
    {
        packageElement->SetAttribute("version", version);
    }
    packageElement->SetAttribute("compression", wing_package::CompressionStr(compression));
    if (!iconFilePath.empty())
    {
        packageElement->SetAttribute("iconFilePath", iconFilePath);
    }
    packageElement->SetAttribute("includeUninstaller", includeUninstaller ? "true" : "false");
    if (!id.is_nil())
    {
        packageElement->SetAttribute("id", util::ToString(id));
    }
}

void Properties::SetSourceRootDir(const std::string& sourceRootDir_)
{
    sourceRootDir = sourceRootDir_;
}

void Properties::SetTargetRootDir(const std::string& targetRootDir_)
{
    targetRootDir = targetRootDir_;
}

void Properties::SetAppName(const std::string& appName_)
{
    appName = appName_;
}

void Properties::SetPublisher(const std::string& publisher_)
{
    publisher = publisher_;
}

void Properties::SetVersion(const std::string& version_)
{
    version = version_;
}

void Properties::SetCompression(wing_package::Compression compression_)
{
    compression = compression_;
}

void Properties::SetIconFilePath(const std::string& iconFilePath_)
{
    iconFilePath = iconFilePath_;
}

void Properties::SetIncludeUninstaller(bool includeUninstaller_)
{
    includeUninstaller = includeUninstaller_;
}

void Properties::SetId(const util::uuid& id_)
{
    id = id_;
}

wing::TreeViewNode* Properties::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode("Properties");
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("package.properties.bitmap"));
    }
    return node;
}

wing::Control* Properties::CreateView(wing::ImageList* imageList)
{
    std::unique_ptr<PropertiesView> viewPtr(new PropertiesView(GetPackage()));
    return viewPtr.release();
}

Package::Package(const std::string& packageXMLFilePath) : Node(NodeKind::package, std::string()), filePath(packageXMLFilePath), view(nullptr), explorer(nullptr), dirty(false), building(false)
{
    properties.reset(new Properties());
    properties->SetParent(this);
    components.reset(new Components());
    components->SetParent(this);
    environment.reset(new Environment());
    environment->SetParent(this);
    links.reset(new Links());
    links->SetParent(this);
    engineVariables.reset(new EngineVariables());
    engineVariables->SetParent(this);
}

Package::Package(const std::string& packageXMLFilePath, xml::Element* root) :
    Node(NodeKind::package, std::string()), filePath(packageXMLFilePath), view(nullptr), explorer(nullptr), dirty(false), building(false)
{
    properties.reset(new Properties());
    properties->SetParent(this);
    components.reset(new Components());
    components->SetParent(this);
    environment.reset(new Environment());
    environment->SetParent(this);
    links.reset(new Links());
    links->SetParent(this);
    if (root->Name() != "package")
    {
        throw PackageXMLException("not a Wingstall package XML file: the root element is not 'package' element", packageXMLFilePath, root);
    }
    std::string nameAttr = root->GetAttribute("name");
    if (!nameAttr.empty())
    {
        SetName(nameAttr);
    }
    else
    {
        throw PackageXMLException("'package' element has no 'name' attribute", packageXMLFilePath, root);
    }
    std::string sourceRootDirAttr = root->GetAttribute("sourceRootDir");
    if (!sourceRootDirAttr.empty())
    {
        properties->SetSourceRootDir(sourceRootDirAttr);
    }
    else
    {
        throw PackageXMLException("'package' element has no 'sourceRootDir' attribute", packageXMLFilePath, root);
    }
    std::string targetRootDirAttr = root->GetAttribute("targetRootDir");
    if (!targetRootDirAttr.empty())
    {
        properties->SetTargetRootDir(targetRootDirAttr);
    }
    else
    {
        throw PackageXMLException("'package' element has no 'targetRootDir' attribute", packageXMLFilePath, root);
    }
    std::string appNameAttr = root->GetAttribute("appName");
    if (!appNameAttr.empty())
    {
        properties->SetAppName(appNameAttr);
    }
    else
    {
        throw PackageXMLException("'package' element has no 'appName' attribute", packageXMLFilePath, root);
    }
    std::string publisherAttr = root->GetAttribute("publisher");
    if (!publisherAttr.empty())
    {
        properties->SetPublisher(publisherAttr);
    }
    std::string versionAttr = root->GetAttribute("version");
    if (!versionAttr.empty())
    {
        properties->SetVersion(versionAttr);
    }
    else
    {
        properties->SetVersion("1.0.0");
    }
    std::string compressionAttr = root->GetAttribute("compression");
    if (!compressionAttr.empty())
    {
        try
        {
            properties->SetCompression(wing_package::ParseCompressionStr(compressionAttr));
        }
        catch (const std::exception& ex)
        {
            throw PackageXMLException("'package' element has invalid 'compression' attribute value: " + std::string(ex.what()), packageXMLFilePath, root);
        }
    }
    std::string iconFilePathAttr = root->GetAttribute("iconFilePath");
    if (!iconFilePathAttr.empty())
    {
        properties->SetIconFilePath(iconFilePathAttr);
    }
    std::string includeUninstallerAttr = root->GetAttribute("includeUninstaller");
    if (!includeUninstallerAttr.empty())
    {
        try
        {
            properties->SetIncludeUninstaller(util::ParseBool(includeUninstallerAttr));
        }
        catch (const std::exception& ex)
        {
            throw PackageXMLException("'package' element has invalid 'includeUninstaller' attribute value: " + std::string(ex.what()), packageXMLFilePath, root);
        }
    }
    std::string idAttr = root->GetAttribute("id");
    if (!idAttr.empty())
    {
        properties->SetId(util::ParseUuid(idAttr));
    }
    else
    {
        properties->SetId(util::random_uuid());
    }
    std::unique_ptr<xpath::XPathObject> preinstallObject = xpath::Evaluate("preinstall", root);
    if (preinstallObject)
    {
        if (preinstallObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(preinstallObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    preinstallComponent.reset(new PreinstallComponent(packageXMLFilePath, element));
                }
            }
            else if (n > 1)
            {
                throw PackageXMLException("'package' element should contain at most one 'preinstall' element", packageXMLFilePath, root);
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> componentObject = xpath::Evaluate("component", root);
    if (componentObject)
    {
        if (componentObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(componentObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    Component* component = new Component(packageXMLFilePath, element);
                    components->AddComponent(component);
                }
            }
        }
    }
    components->MakeDisjoint();
    components->MakeMaps();
    std::unique_ptr<xpath::XPathObject> environmentObject = xpath::Evaluate("environment", root);
    if (environmentObject)
    {
        if (environmentObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(environmentObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    environment.reset(new Environment(packageXMLFilePath, element));
                    environment->SetParent(this);
                }
            }
            else if (n > 1)
            {
                throw PackageXMLException("'package' element should contain at most one 'environment' element", packageXMLFilePath, root);
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> linksObject = xpath::Evaluate("links", root);
    if (linksObject)
    {
        if (linksObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(linksObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    links.reset(new Links(packageXMLFilePath, element));
                    links->SetParent(this);
                }
            }
            else if (n > 1)
            {
                throw PackageXMLException("'package' element should contain at most one 'links' element", packageXMLFilePath, root);
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> uninstallObject = xpath::Evaluate("uninstall", root);
    if (uninstallObject)
    {
        if (uninstallObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(uninstallObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    uninstallComponent.reset(new UninstallComponent(packageXMLFilePath, element));
                }
            }
            else if (n > 1)
            {
                throw PackageXMLException("'package' element should contain at most one 'uninstall' element", packageXMLFilePath, root);
            }
        }

    }
    engineVariables.reset(new EngineVariables());
    engineVariables->SetParent(this);
}

bool Package::CanSave() const
{
    return IsDirty();
}

void Package::Save()
{
    MainWindow* mainWindow = GetMainWindow();
    xml::Document packageDoc;
    packageDoc.AppendChild(ToXml());
    std::string directory = util::Path::GetDirectoryName(filePath);
    std::filesystem::create_directories(directory);
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    packageDoc.Write(formatter);
    dirty = false;
    if (mainWindow)
    {
        mainWindow->DisableSave();
    }
}

bool Package::CanBuild() const
{
    return true;
}

void Package::Build()
{
    building = true;
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        mainWindow->StartBuild();
    }
}

void Package::Close()
{
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        mainWindow->ClosePackageClick();
    }
}

void Package::SetDirty()
{
    MainWindow* mainWindow = GetMainWindow();
    if (mainWindow)
    {
        mainWindow->EnableSave();
    }
    dirty = true;
}

void Package::ResetBuilding()
{
    building = false;
}

xml::Element* Package::ToXml() const
{
    xml::Element* element = xml::MakeElement("package");
    element->SetAttribute("name", Name());
    properties->SetAttributes(element);
    if (preinstallComponent)
    {
        element->AppendChild(preinstallComponent->ToXml());
    }
    components->AddElements(element);
    element->AppendChild(environment->ToXml());
    element->AppendChild(links->ToXml());
    if (uninstallComponent)
    {
        element->AppendChild(uninstallComponent->ToXml());
    }
    return element;
}

std::string Package::BinDirectoryPath() const
{
    std::string packageDirectoryPath = util::Path::GetDirectoryName(filePath);
    return util::Path::Combine(packageDirectoryPath, "bin");
}

wing::TreeViewNode* Package::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* root = new wing::TreeViewNode("Package '" + Name() + "'");
    SetTreeViewNode(root);
    root->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        root->SetImageIndex(imageList->GetImageIndex("package.bitmap"));
    }
    root->AddChild(properties->ToTreeViewNode(view));
    root->AddChild(components->ToTreeViewNode(view));
    root->AddChild(environment->ToTreeViewNode(view));
    root->AddChild(links->ToTreeViewNode(view));
    root->AddChild(engineVariables->ToTreeViewNode(view));
    return root;
}

wing::Control* Package::CreateView(wing::ImageList* imageList)
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
    wing::ListViewItem* propertiesItem = listView->AddItem();
    properties->SetData(propertiesItem, imageList);
    wing::ListViewItem* componentsItem = listView->AddItem();
    components->SetData(componentsItem, imageList);
    wing::ListViewItem* environmentItem = listView->AddItem();
    environment->SetData(environmentItem, imageList);
    wing::ListViewItem* linksItem = listView->AddItem();
    links->SetData(linksItem, imageList);
    wing::ListViewItem* engineVariablesItem = listView->AddItem();
    engineVariables->SetData(engineVariablesItem, imageList);
    return listView.release();
}

} // package_editor

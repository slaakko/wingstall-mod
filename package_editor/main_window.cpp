// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef MessageBox

module package_editor.main_window;

import package_editor.new_package_dialog;
import package_editor.path_bar;
import package_editor.about_dialog;
import package_editor.edit_configuration_dialog;
import package_editor.configuration;
import wingstall_config;
import dom;
import util;
import std.filesystem;

namespace package_editor {

std::string WingstallPackagesDir()
{
    std::string packagesDir = util::GetFullPath(util::Path::Combine(util::WingstallRoot(), "packages"));
    std::filesystem::create_directories(packagesDir);
    return packagesDir;
}

MainWindow::MainWindow() : wing::Window(wing::WindowCreateParams().Text("Wingstall Package Editor").WindowClassName("package_editor.main_window")),
    newPackageMenuItem(nullptr),
    openPackageMenuItem(nullptr),
    closePackageMenuItem(nullptr),
    exitMenuItem(nullptr),
    packageExplorer(nullptr),
    packageContentView(nullptr),
    logView(nullptr),
    showingDialog(false),
    canceled(false),
    setMaximizedSplitterDistance(false)
{
    std::unique_ptr<wing::MenuBar> menuBar(new wing::MenuBar());

    std::unique_ptr<wing::MenuItem> fileMenuItem(new wing::MenuItem("&File"));

    std::unique_ptr<wing::MenuItem> newPackageMenuItemPtr(new wing::MenuItem("&New Package..."));
    newPackageMenuItem = newPackageMenuItemPtr.get();
    newPackageMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::n);
    newPackageMenuItem->Click().AddHandler(this, &MainWindow::NewPackageClick);
    fileMenuItem->AddMenuItem(newPackageMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> openPackageMenuItemPtr(new wing::MenuItem("&Open Package..."));
    openPackageMenuItem = openPackageMenuItemPtr.get();
    openPackageMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::o);
    openPackageMenuItem->Click().AddHandler(this, &MainWindow::OpenPackageClick);
    fileMenuItem->AddMenuItem(openPackageMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> savePackageMenuItemPtr(new wing::MenuItem("&Save Package"));
    savePackageMenuItem = savePackageMenuItemPtr.get();
    savePackageMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::s);
    savePackageMenuItem->Click().AddHandler(this, &MainWindow::SavePackageClick);
    savePackageMenuItem->Disable();
    fileMenuItem->AddMenuItem(savePackageMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> closePackageMenuItemPtr(new wing::MenuItem("&Close Package"));
    closePackageMenuItem = closePackageMenuItemPtr.get();
    closePackageMenuItem->Click().AddHandler(this, &MainWindow::ClosePackageClick);
    closePackageMenuItem->Disable();
    fileMenuItem->AddMenuItem(closePackageMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> exitMenuItemPtr(new wing::MenuItem("E&xit"));
    exitMenuItem = exitMenuItemPtr.get();
    exitMenuItem->Click().AddHandler(this, &MainWindow::ExitClick);
    fileMenuItem->AddMenuItem(exitMenuItemPtr.release());

    menuBar->AddMenuItem(fileMenuItem.release());

    std::unique_ptr<wing::MenuItem> editMenuItem(new wing::MenuItem("&Edit"));

    std::unique_ptr<wing::MenuItem> editConfigurationMenuItemPtr(new wing::MenuItem("&Configuration..."));
    editConfigurationMenuItem = editConfigurationMenuItemPtr.get();
    editConfigurationMenuItem->Click().AddHandler(this, &MainWindow::EditConfigurationClick);
    editMenuItem->AddMenuItem(editConfigurationMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> resetMenuItemPtr(new wing::MenuItem("&Reset UI values to defaults"));
    resetMenuItem = resetMenuItemPtr.get();
    resetMenuItem->Click().AddHandler(this, &MainWindow::ResetConfigClick);
    editMenuItem->AddMenuItem(resetMenuItemPtr.release());

    menuBar->AddMenuItem(editMenuItem.release());

    std::unique_ptr<wing::MenuItem> buildMenuItem(new wing::MenuItem("&Build"));

    std::unique_ptr<wing::MenuItem> buildPackageMenuItemPtr(new wing::MenuItem("&Build Package"));
    buildPackageMenuItem = buildPackageMenuItemPtr.get();
    buildPackageMenuItem->SetShortcut(wing::Keys::f5);
    buildPackageMenuItem->Click().AddHandler(this, &MainWindow::BuildPackageClick);
    buildPackageMenuItem->Disable();
    buildMenuItem->AddMenuItem(buildPackageMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> openBinDirectoryMenuItemPtr(new wing::MenuItem("&Open Bin Directory"));
    openBinDirectoryMenuItem = openBinDirectoryMenuItemPtr.get();
    openBinDirectoryMenuItem->SetShortcut(wing::Keys::f7);
    openBinDirectoryMenuItem->Click().AddHandler(this, &MainWindow::OpenBinDirectoryClick);
    openBinDirectoryMenuItem->Disable();
    buildMenuItem->AddMenuItem(openBinDirectoryMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> cancelBuildMenuItemPtr(new wing::MenuItem("&Cancel Build"));
    cancelBuildMenuItem = cancelBuildMenuItemPtr.get();
    cancelBuildMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::f4);
    cancelBuildMenuItem->Click().AddHandler(this, &MainWindow::CancelBuildClick);
    cancelBuildMenuItem->Disable();
    buildMenuItem->AddMenuItem(cancelBuildMenuItemPtr.release());

    menuBar->AddMenuItem(buildMenuItem.release());

    std::unique_ptr<wing::MenuItem> helpMenuItem(new wing::MenuItem("&Help"));

    std::unique_ptr<wing::MenuItem> homepageMenuItemPtr(new wing::MenuItem("&Homepage"));
    homepageMenuItem = homepageMenuItemPtr.get();
    homepageMenuItem->Click().AddHandler(this, &MainWindow::HomepageClick);
    helpMenuItem->AddMenuItem(homepageMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> localDocumentationMenuItemPtr(new wing::MenuItem("&Local Documentation"));
    localDocumentationMenuItem = localDocumentationMenuItemPtr.get();
    localDocumentationMenuItem->Click().AddHandler(this, &MainWindow::LocalDocumentationClick);
    helpMenuItem->AddMenuItem(localDocumentationMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> aboutMenuItemPtr(new wing::MenuItem("&About..."));
    aboutMenuItem = aboutMenuItemPtr.get();
    aboutMenuItem->Click().AddHandler(this, &MainWindow::AboutClick);
    helpMenuItem->AddMenuItem(aboutMenuItemPtr.release());

    menuBar->AddMenuItem(helpMenuItem.release());

    AddChild(menuBar.release());

    std::unique_ptr<wing::ToolBar> toolBarPtr(MakeToolBar());
    wing::ToolBar* toolBar = toolBarPtr.get();
    std::unique_ptr<wing::Control> borderedToolBar(new wing::BorderedControl(wing::BorderedControlCreateParams(toolBarPtr.release()).SetBorderStyle(wing::BorderStyle::single).
        NormalSingleBorderColor(DefaultToolBarBorderColor()).FocusedSingleBorderColor(DefaultToolBarBorderColor()).SetSize(toolBar->GetSize()).SetDock(wing::Dock::top)));

    std::unique_ptr<wing::ToolButton> saveToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName("save.bitmap").
        SetPadding(wing::Padding(8, 8, 8, 8)).SetToolTip("Save Package (Ctrl+S)")));
    saveToolButton = saveToolButtonPtr.get();
    saveToolButton->Click().AddHandler(this, &MainWindow::SavePackageClick);
    saveToolButton->Disable();
    toolBar->AddToolButton(saveToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> buildToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName("build.bitmap").
        SetPadding(wing::Padding(6, 6, 6, 6)).SetToolTip("Build Package (F5)")));
    buildToolButton = buildToolButtonPtr.get();
    buildToolButton->Click().AddHandler(this, &MainWindow::BuildPackageClick);
    buildToolButton->Disable();
    toolBar->AddToolButton(buildToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> cancelBuildToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName("cancel.build.bitmap").
        SetPadding(wing::Padding(6, 6, 6, 6)).SetToolTip("Cancel Build (Ctrl+F4)")));
    cancelBuildToolButton = cancelBuildToolButtonPtr.get();
    cancelBuildToolButton->Click().AddHandler(this, &MainWindow::CancelBuildClick);
    cancelBuildToolButton->Disable();
    toolBar->AddToolButton(cancelBuildToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> openBinDirectoryToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName("folder.opened.bitmap").
        SetPadding(wing::Padding(6, 6, 6, 6)).SetToolTip("Open Bin Directory (F7)")));
    openBinDirectoryToolButton = openBinDirectoryToolButtonPtr.get();
    openBinDirectoryToolButton->Click().AddHandler(this, &MainWindow::OpenBinDirectoryClick);
    openBinDirectoryToolButton->Disable();
    toolBar->AddToolButton(openBinDirectoryToolButtonPtr.release());

    AddChild(borderedToolBar.release());

    std::unique_ptr<PathBar> pathBarPtr(new PathBar(PathBarCreateParams(&imageList).Defaults()));
    pathBar = pathBarPtr.get();
    pathBar->GetParentPathSelector()->Click().AddHandler(this, &MainWindow::ParentPathSelectorClick);
    pathBar->GetPathView()->PathComponentSelected().AddHandler(this, &MainWindow::PathComponentSelected);
    std::unique_ptr<wing::BorderedControl> borderedPathBar(new wing::BorderedControl(wing::BorderedControlCreateParams(pathBarPtr.release()).SetSize(wing::Size(0, 32)).
        SetDock(wing::Dock::top).NormalSingleBorderColor(DefaultPathBarFrameColor())));
    AddChild(borderedPathBar.release());

    std::unique_ptr<wing::SplitContainer> verticalSplitContainerPtr(new wing::SplitContainer(wing::SplitContainerCreateParams(wing::SplitterOrientation::vertical).
        SetDock(wing::Dock::fill)));
    verticalSplitContainer = verticalSplitContainerPtr.get();

    std::unique_ptr<wing::SplitContainer> horizontalSplitContainerPtr(new wing::SplitContainer(wing::SplitContainerCreateParams(wing::SplitterOrientation::horizontal).
        SetDock(wing::Dock::fill).SplitterDistance(500)));
    horizontalSplitContainer = horizontalSplitContainerPtr.get();
    std::unique_ptr<PackageExplorer> packageExplorerPtr(new PackageExplorer(PackageExplorerCreateParams().SetDock(wing::Dock::fill)));
    packageExplorer = packageExplorerPtr.get();
    packageExplorer->SetMainWindow(this);
    horizontalSplitContainer->Pane1Container()->AddChild(packageExplorerPtr.release());
    packageExplorer->SizeChanged().AddHandler(this, &MainWindow::PackageExplorerSizeChanged);

    std::unique_ptr<PackageContentView> packageContentViewPtr(new PackageContentView(PackageContentViewCreateParams().SetDock(wing::Dock::fill)));
    packageContentView = packageContentViewPtr.get();
    packageContentView->SetMainWindow(this);
    packageContentView->SizeChanged().AddHandler(this, &MainWindow::PackageContentViewSizeChanged);
    horizontalSplitContainer->Pane2Container()->AddChild(packageContentViewPtr.release());

    packageExplorer->SetContentView(packageContentView);

    verticalSplitContainer->Pane1Container()->AddChild(horizontalSplitContainerPtr.release());

    std::unique_ptr<wing::LogView> logViewPtr(new wing::LogView(wing::TextViewCreateParams().Defaults()));
    logView = logViewPtr.get();
    logView->SetFlag(wing::ControlFlags::scrollSubject);
    logView->SetDoubleBuffered();
    std::unique_ptr<wing::Control> paddedLogView(new wing::PaddedControl(wing::PaddedControlCreateParams(logViewPtr.release()).Defaults()));
    std::unique_ptr<wing::Control> scrollableLogView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedLogView.release()).SetDock(wing::Dock::fill)));
    wing::Application::SetLogView(logView);
    verticalSplitContainer->Pane2Container()->AddChild(scrollableLogView.release());

    AddChild(verticalSplitContainerPtr.release());

    imageList.AddImage("package.bitmap");
    imageList.AddImage("components.bitmap");
    imageList.AddImage("component.bitmap");
    imageList.AddImage("component.bitmap");
    imageList.AddImage("add.folder.bitmap");
    imageList.AddImage("add.file.bitmap");
    imageList.AddImage("delete.folder.bitmap");
    imageList.AddImage("delete.folder.cascade.bitmap");
    imageList.AddImage("delete.file.bitmap");
    imageList.AddImage("delete.file.cascade.bitmap");
    imageList.AddImage("folder.closed.bitmap");
    imageList.AddDisabledImage("folder.closed.bitmap");
    imageList.AddImage("folder.opened.bitmap");
    imageList.AddImage("file.bitmap");
    imageList.AddDisabledImage("file.bitmap");
    imageList.AddImage("rules.bitmap");
    imageList.AddImage("document.collection.bitmap");
    imageList.AddImage("package.properties.bitmap");
    imageList.AddImage("environment.bitmap");
    imageList.AddImage("environment.var.bitmap");
    imageList.AddImage("path.directory.bitmap");
    imageList.AddImage("links.bitmap");
    imageList.AddImage("linked.folder.closed.bitmap");
    imageList.AddImage("shortcut.bitmap");
    imageList.AddImage("engine.variables.bitmap");
    imageList.AddImage("engine.variable.bitmap");
    imageList.AddImage("xml.file.bitmap");
    imageList.AddImage("up.arrow.bitmap");

    packageExplorer->SetImageList(&imageList);
    packageContentView->SetImageList(&imageList);

    std::unique_ptr<wing::StatusBar> statusBarPtr(new wing::StatusBar(wing::StatusBarCreateParams().Defaults()));
    wing::StatusBar* statusBar = statusBarPtr.get();
    statusBar->SetDoubleBuffered();

    std::unique_ptr<wing::StatusBarTextItem> packageFilePathLabelStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().Text("File Path:")));
    packageFilePathLabelStatusBarItem = packageFilePathLabelStatusBarItemPtr.get();
    packageFilePathLabelStatusBarItem->SetVisible(false);
    statusBar->AddItem(packageFilePathLabelStatusBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> packageFilePathStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().
        BorderStyle(wing::StatusBarItemBorderStyle::sunken)));
    packageFilePathStatusBarItem = packageFilePathStatusBarItemPtr.get();
    packageFilePathStatusBarItem->SetVisible(false);
    statusBar->AddItem(packageFilePathStatusBarItemPtr.release());

    std::unique_ptr<wing::StatusBarTextItem> packageBuildProgressLabelStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().Text("Build Progress:")));
    packageBuildProgressLabelStatusBarItem = packageBuildProgressLabelStatusBarItemPtr.get();
    packageBuildProgressLabelStatusBarItem->SetVisible(false);
    statusBar->AddItem(packageBuildProgressLabelStatusBarItemPtr.release());
    std::unique_ptr<wing::ProgressBar> progressBarPtr(new wing::ProgressBar(wing::ProgressBarCreateParams().Defaults()));
    buildProgressBar = progressBarPtr.get();
    buildProgressBar->SetDoubleBuffered();
    buildProgressBar->SetSize(wing::Size(300, 24));
    buildProgressBar->Hide();
    std::unique_ptr<wing::StatusBarControlItem> buildProgressStatusBarItemPtr(new wing::StatusBarControlItem(wing::StatusBarControlItemCreateParams(progressBarPtr.release()).Defaults()));
    buildProgressStatusBarItem = buildProgressStatusBarItemPtr.get();
    buildProgressStatusBarItem->SetVisible(false);
    statusBar->AddItem(buildProgressStatusBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> packageBuildProgressPerceStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().
        BorderStyle(wing::StatusBarItemBorderStyle::sunken).MaxTextLength(3)));
    packageBuildProgressPerceStatusBarItem = packageBuildProgressPerceStatusBarItemPtr.get();
    packageBuildProgressPerceStatusBarItem->SetVisible(false);
    statusBar->AddItem(packageBuildProgressPerceStatusBarItemPtr.release());

    AddChild(statusBarPtr.release());

    SetCommandStatus();

    LoadConfigurationSettings();

    SetTimer(configurationSaveTimerId, configurationSavePeriod);
}

MainWindow::~MainWindow()
{
    if (IsConfigurationChanged())
    {
        SaveConfiguration();
    }
    if (buildThread)
    {
        if (buildThread->joinable())
        {
            buildThread->join();
        }
    }
    packageContentView->ViewContent(nullptr);
}

void MainWindow::AddListViewEventHandlers(wing::ListView* listView)
{
    listView->ItemClick().AddHandler(this, &MainWindow::ListViewItemClick);
    listView->ItemRightClick().AddHandler(this, &MainWindow::ListViewItemRightClick);
    listView->ItemDoubleClick().AddHandler(this, &MainWindow::ListViewItemDoubleClick);
}

void MainWindow::AddTreeViewEventHandlers(wing::TreeView* treeView)
{
    treeView->NodeClick().AddHandler(packageExplorer, &PackageExplorer::TreeViewNodeClick);
}

void MainWindow::ClearClickActions()
{
    clickActions.clear();
}

void MainWindow::ShowPackageFilePathStatusItems()
{
    packageFilePathLabelStatusBarItem->SetVisible(true);
    packageFilePathStatusBarItem->SetVisible(true);
}

void MainWindow::HidePackageFilePathStatusItems()
{
    packageFilePathLabelStatusBarItem->SetVisible(false);
    packageFilePathLabelStatusBarItem->SetVisible(false);
}

void MainWindow::EnableSave()
{
    saveToolButton->Enable();
    savePackageMenuItem->Enable();
}

void MainWindow::DisableSave()
{
    saveToolButton->Disable();
    savePackageMenuItem->Disable();
}

void MainWindow::ShowPackageBuildProgressStatusItems()
{
    packageBuildProgressLabelStatusBarItem->SetVisible(true);
    buildProgressStatusBarItem->SetVisible(true);
    buildProgressBar->Show();
    packageBuildProgressPerceStatusBarItem->SetVisible(true);
}

void MainWindow::HidePackageBuildProgressStatusItems()
{
    packageBuildProgressLabelStatusBarItem->SetVisible(false);
    buildProgressStatusBarItem->SetVisible(false);
    buildProgressBar->Hide();
    packageBuildProgressPerceStatusBarItem->SetVisible(false);
}

void MainWindow::StartBuild()
{
    if (buildThread)
    {
        if (buildThread->joinable())
        {
            buildThread->join();
        }
    }
    if (package->IsDirty())
    {
        package->Save();
    }
    ShowPackageBuildProgressStatusItems();
    buildTask.reset(new BuildTask(this));
    buildThread.reset(new std::thread(RunBuildTask, buildTask.get()));
}

void MainWindow::CancelBuildClick()
{
    if (buildTask)
    {
        buildTask->Cancel();
    }
}

void MainWindow::EndBuild()
{
    HidePackageBuildProgressStatusItems();
    package->ResetBuilding();
    SetCommandStatus();
}

void MainWindow::OnTimer(wing::TimerEventArgs& args)
{
    if (args.timerId == configurationSaveTimerId)
    {
        SetWindowSettings();
        if (IsConfigurationChanged())
        {
            SaveConfiguration();
        }
    }
}

void MainWindow::OnKeyDown(wing::KeyEventArgs& args)
{
    try
    {
        Window::OnKeyDown(args);
        if (!args.handled)
        {
            switch (args.key)
            {
            case wing::Keys::escape:
            {
                RemoveContextMenu();
                args.handled = true;
                break;
            }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::MouseUpNotification(wing::MouseEventArgs& args)
{
    try
    {
        if (args.buttons == wing::MouseButtons::lbutton)
        {
            if (!showingDialog)
            {
                RemoveContextMenu();
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::OnWindowStateChanged()
{
    Window::OnWindowStateChanged();
    if (GetWindowState() == wing::WindowState::normal)
    {
        if (IsNormalWindowSettingsDefined())
        {
            SetLocation(GetConfiguredWindowLocation());
            SetSize(GetConfiguredWindowSize());
            horizontalSplitContainer->SetSplitterDistance(GetConfiguredNormalHorizontalSplitterDistance());
            verticalSplitContainer->SetSplitterDistance(GetConfiguredNormalVerticalSplitterDistance());
        }
    }
    else if (GetWindowState() == wing::WindowState::maximized)
    {
        setMaximizedSplitterDistance = true;
    }
}

void MainWindow::OnSizeChanged()
{
    Window::OnSizeChanged();
    if (GetWindowState() == wing::WindowState::maximized && setMaximizedSplitterDistance)
    {
        setMaximizedSplitterDistance = false;
        if (IsMaximizedWindowSettingsDefined())
        {
            horizontalSplitContainer->SetSplitterDistance(GetConfiguredMaximizedHorizontalSplitterDistance());
            verticalSplitContainer->SetSplitterDistance(GetConfiguredMaximizedVerticalSplitterDistance());
        }
    }
}

void MainWindow::OnWindowClosing(wing::CancelArgs& args)
{
    ClosePackageClick();
    if (canceled)
    {
        canceled = false;
        args.cancel = true;
    }
}

void MainWindow::PackageExplorerSizeChanged()
{
    wing::Size packageExplorerSize = packageExplorer->GetSize();
    pathBar->SetDirectoryPathViewWidth(packageExplorerSize.Width);
}

void MainWindow::PackageContentViewSizeChanged()
{
    wing::Size packageContentViewSize = packageContentView->GetSize();
    pathBar->SetPathViewMaxWidth(packageContentViewSize.Width);
}

void MainWindow::PathComponentSelected(wing::PathComponentArgs& args)
{
    wing::PathComponent* pathComponent = args.pathComponent;
    if (pathComponent)
    {
        void* data = pathComponent->Data();
        if (data)
        {
            Node* node = static_cast<Node*>(data);
            node->Open();
        }
    }
}

void MainWindow::ListViewItemClick(wing::ListViewItemEventArgs& args)
{
    if (args.item)
    {
        wing::ListView* view = args.item->View();
        if (view)
        {
            view->SetSelectedItem(args.item);
        }
    }
}

void MainWindow::ListViewItemRightClick(wing::ListViewItemEventArgs& args)
{
    if (args.item)
    {
        wing::ListView* view = args.view;
        if (view)
        {
            view->SetSelectedItem(args.item);
            void* data = args.item->Data();
            if (data)
            {
                Node* node = static_cast<Node*>(data);
                ClearClickActions();
                std::unique_ptr<wing::ContextMenu> contextMenu(new wing::ContextMenu());
                node->AddMenuItems(contextMenu.get(), clickActions, MenuItemsKind::allMenuItems);
                if (contextMenu->HasMenuItems())
                {
                    view->TranslateContentLocation(args.location);
                    wing::Point screenLoc = view->ClientToScreen(args.location);
                    ShowContextMenu(contextMenu.release(), screenLoc);
                }
            }
        }
    }
    else
    {
        Node* node = packageExplorer->SelectedNode();
        if (node)
        {
            ClearClickActions();
            std::unique_ptr<wing::ContextMenu> contextMenu(new wing::ContextMenu());
            node->AddMenuItems(contextMenu.get(), clickActions, MenuItemsKind::newMenuItems);
            if (contextMenu->HasMenuItems())
            {
                wing::ListView* view = args.view;
                if (view)
                {
                    view->TranslateContentLocation(args.location);
                    wing::Point screenLoc = view->ClientToScreen(args.location);
                    ShowContextMenu(contextMenu.release(), screenLoc);
                }
            }
        }
    }
}

void MainWindow::ListViewItemDoubleClick(wing::ListViewItemEventArgs& args)
{
    if (args.item && args.item->Data())
    {
        Node* node = static_cast<Node*>(args.item->Data());
        node->ExecuteDefaultAction();
    }
}

void MainWindow::ParentPathSelectorClick()
{
    Node* selectedNode = packageExplorer->SelectedNode();
    if (selectedNode)
    {
        Node* parent = selectedNode->Parent();
        if (parent)
        {
            parent->Open();
        }
    }
}

void MainWindow::LoadConfigurationSettings()
{
    LoadConfiguration();
    SetWindowState(GetConfiguredWindowState());
    if (GetWindowState() == wing::WindowState::normal)
    {
        if (IsNormalWindowSettingsDefined())
        {
            SetLocation(GetConfiguredWindowLocation());
            SetSize(GetConfiguredWindowSize());
            horizontalSplitContainer->SetSplitterDistance(GetConfiguredNormalHorizontalSplitterDistance());
            verticalSplitContainer->SetSplitterDistance(GetConfiguredNormalVerticalSplitterDistance());
        }
    }
    else if (GetWindowState() == wing::WindowState::maximized)
    {
        setMaximizedSplitterDistance = true;
    }
}

void MainWindow::SetWindowSettings()
{
    wing::WindowState windowState = GetWindowState();
    package_editor::SetWindowState(windowState);
    if (windowState == wing::WindowState::normal)
    {
        wing::Point location = Location();
        SetWindowLocation(location);
        wing::Size size = GetSize();
        SetWindowSize(size);
        int horizontalSplitterDistance = horizontalSplitContainer->SplitterDistance();
        SetNormalHorizontalSplitterDistance(horizontalSplitterDistance);
        int verticalSplitterDistance = verticalSplitContainer->SplitterDistance();
        SetNormalVerticalSplitterDistance(verticalSplitterDistance);
        SetNormalWindowSettingsDefined();
    }
    else if (windowState == wing::WindowState::maximized)
    {
        int horizontalSplitterDistance = horizontalSplitContainer->SplitterDistance();
        SetMaximizedHorizontalSplitterDistance(horizontalSplitterDistance);
        int verticalSplitterDistance = verticalSplitContainer->SplitterDistance();
        SetMaximizedVerticalSplitterDistance(verticalSplitterDistance);
        SetMaximizedWindowSettingsDefined();
    }
}

void MainWindow::NewPackageClick()
{
    try
    {
        bool cancel = false;
        wing::CancelArgs cancelArgs(cancel);
        ExitView().Fire(cancelArgs);
        if (cancelArgs.cancel)
        {
            return;
        }
        NewPackageDialog dialog;
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            ClosePackageClick();
            if (canceled)
            {
                canceled = false;
                return;
            }
            package.reset(new Package(dialog.GetPackageFilePath()));
            packageFilePathStatusBarItem->SetText(dialog.GetPackageFilePath());
            ShowPackageFilePathStatusItems();
            package->SetName(dialog.GetPackageName());
            package->SetContentView(packageContentView);
            package->SetExplorer(packageExplorer);
            packageExplorer->SetPackage(package.get());
            package->Open();
            package->GetProperties()->SetSourceRootDir(util::GetFullPath("C:/"));
            package->GetProperties()->SetTargetRootDir("C:/" + dialog.GetPackageName());
            package->GetProperties()->SetAppName(dialog.GetPackageName());
            package->GetProperties()->SetVersion("1.0");
            package->GetProperties()->SetCompression(wing_package::Compression::deflate);
            package->GetProperties()->SetId(util::random_uuid());
            package->GetEngineVariables()->Fetch();
            package->SetDirty();
            SetCommandStatus();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::OpenPackageClick()
{
    try
    {
        bool cancel = false;
        wing::CancelArgs cancelArgs(cancel);
        ExitView().Fire(cancelArgs);
        if (cancelArgs.cancel)
        {
            return;
        }
        std::vector<std::pair<std::string, std::string>> descriptionFilterPairs;
        descriptionFilterPairs.push_back(std::make_pair("Package Files (*.package.xml)", "*.package.xml"));
        descriptionFilterPairs.push_back(std::make_pair("XML Files (*.xml)", "*.xml"));
        descriptionFilterPairs.push_back(std::make_pair("All Files (*.*)", "*.*"));
        std::string initialDirectory = WingstallPackagesDir();
        std::string filePath;
        std::string currentDirectory;
        std::vector<std::string> fileNames;
        bool selected = wing::OpenFileName(Handle(), descriptionFilterPairs, initialDirectory, std::string(), "xml", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filePath, currentDirectory, fileNames);
        if (selected)
        {
            ClosePackageClick();
            if (canceled)
            {
                canceled = false;
                return;
            }
            std::string packageXMLFilePath = util::GetFullPath(filePath);
            std::unique_ptr<xml::Document> packageDoc(xml::ReadDocument(packageXMLFilePath));
            package.reset(new Package(packageXMLFilePath, packageDoc->DocumentElement()));
            packageFilePathStatusBarItem->SetText(packageXMLFilePath);
            ShowPackageFilePathStatusItems();
            package->SetContentView(packageContentView);
            package->SetExplorer(packageExplorer);
            packageExplorer->SetPackage(package.get());
            package->Open();
            SetCommandStatus();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::ClosePackageClick()
{
    try
    {
        bool cancel = false;
        wing::CancelArgs cancelArgs(cancel);
        ExitView().Fire(cancelArgs);
        if (cancelArgs.cancel)
        {
            canceled = true;
            return;
        }
        if (package)
        {
            if (package->IsDirty())
            {
                wing::MessageBoxResult result = wing::MessageBox::Show("Save package?", "Package '" + package->Name() + "' has been modified", this, MB_YESNOCANCEL);
                if (result == wing::MessageBoxResult::cancel)
                {
                    canceled = true;
                    return;
                }
                else if (result == wing::MessageBoxResult::yes)
                {
                    package->Save();
                }
            }
        }
        package.reset();
        packageFilePathStatusBarItem->SetText(std::string());
        HidePackageFilePathStatusItems();
        pathBar->SetCurrentNode(nullptr);
        pathBar->SetDirectoryPath(std::string());
        packageExplorer->SetPackage(package.get());
        packageContentView->ViewContent(nullptr);
        SetCommandStatus();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
    canceled = false;
}

void MainWindow::SetBuildProgressPercent(int percent)
{
    buildProgressBar->SetProgressPercent(percent);
    packageBuildProgressPerceStatusBarItem->SetText(std::to_string(percent) + "%");
}

void MainWindow::ListViewColumnWidthChanged(wing::ListViewColumnEventArgs& args)
{
    wing::ListView* listView = args.view;
    void* data = listView->GetData();
    if (data)
    {
        Node* node = static_cast<Node*>(data);
        wing::ListViewColumn* column = args.column;
        if (column)
        {
            View& view = GetConfiguredViewSettings().GetView(node->ViewName());
            ColumnWidth& columnWidth = view.GetColumnWidth(column->Name());
            columnWidth.Set(column->Width());
        }
    }
}

void MainWindow::SavePackageClick()
{
    try
    {
        if (package)
        {
            package->Save();
        }
        SetCommandStatus();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::BuildPackageClick()
{
    try
    {
        package->Build();
        SetCommandStatus();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::OpenBinDirectoryClick()
{
    try
    {
        std::string binDirectoryFilePath = package->BinDirectoryPath();
#pragma warning(disable:4311)
#pragma warning(disable:4302)
        if (reinterpret_cast<int>(ShellExecuteA(Handle(), "open", binDirectoryFilePath.c_str(), nullptr, nullptr, SW_SHOWNORMAL)) < 32)
        {
            throw std::runtime_error("shell execute failed");
        }
#pragma warning(default:4311)
#pragma warning(default:4302)

    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::ExitClick()
{
    try
    {
        ClosePackageClick();
        if (canceled)
        {
            canceled = false;
            return;
        }
        Close();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::EditConfigurationClick()
{
    try
    {
        std::unique_ptr<xml::Document> configDoc = wingstall_config::ConfigurationDocument();
        std::string boostIncludeDir = util::MakeNativePath(wingstall_config::BoostIncludeDir(configDoc.get()));
        std::string boostLibDir = util::MakeNativePath(wingstall_config::BoostLibDir(configDoc.get()));
        std::string vcVarsPath = util::MakeNativePath(wingstall_config::VCVarsFilePath(configDoc.get()));
        std::string vcPlatformToolset = wingstall_config::VCPlatformToolset(configDoc.get());
        EditConfigurationDialog dialog(boostIncludeDir, boostLibDir, vcVarsPath, vcPlatformToolset);
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            dialog.GetData(boostIncludeDir, boostLibDir, vcVarsPath, vcPlatformToolset);
            wingstall_config::SetBoostIncludeDir(configDoc.get(), boostIncludeDir);
            wingstall_config::SetBoostLibDir(configDoc.get(), boostLibDir);
            wingstall_config::SetVCVarsFilePath(configDoc.get(), vcVarsPath);
            wingstall_config::SetVCPlatformToolset(configDoc.get(), vcPlatformToolset);
            wingstall_config::SaveConfiguration(configDoc.get());
            wingstall_config::MakeBuildPropsFile(boostIncludeDir, boostLibDir);
            wing::ShowMessageBox(Handle(), "Information", "Configuration saved to '" + util::GetFullPath(wingstall_config::WingstallConfigDir()) + "' directory");
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::HomepageClick()
{
    try
    {
        std::string homePage = "http://slaakko.github.io/wingstall/";
        ShellExecuteA(Handle(), "open", homePage.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::LocalDocumentationClick()
{
    try
    {
        std::string indexFilePath = util::GetFullPath(util::Path::Combine(util::WingstallRoot(), "doc/index.html"));
        ShellExecuteA(Handle(), "open", indexFilePath.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::AboutClick()
{
    try
    {
        AboutDialog dialog;
        dialog.ShowDialog(*this);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::ResetConfigClick()
{
    try
    {
        ResetConfiguration();
        Node* node = packageExplorer->SelectedNode();
        if (node)
        {
            node->OpenAndExpand();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::SetCommandStatus()
{
    if (package)
    {
        if (package->IsDirty())
        {
            EnableSave();
        }
        else
        {
            DisableSave();
        }
        closePackageMenuItem->Enable();
        buildPackageMenuItem->Enable();
        buildToolButton->Enable();
        if (std::filesystem::exists(package->BinDirectoryPath()))
        {
            openBinDirectoryMenuItem->Enable();
            openBinDirectoryToolButton->Enable();
        }
        pathBar->Show();
        if (package->Building())
        {
            buildPackageMenuItem->Disable();
            cancelBuildMenuItem->Enable();
            cancelBuildToolButton->Enable();
            buildToolButton->Disable();
            closePackageMenuItem->Disable();
            openBinDirectoryMenuItem->Disable();
            savePackageMenuItem->Disable();
            saveToolButton->Disable();
            newPackageMenuItem->Disable();
            openPackageMenuItem->Disable();
            exitMenuItem->Disable();
        }
        else
        {
            buildPackageMenuItem->Enable();
            buildToolButton->Enable();
            cancelBuildMenuItem->Disable();
            cancelBuildToolButton->Disable();
            newPackageMenuItem->Enable();
            openPackageMenuItem->Enable();
            exitMenuItem->Enable();
        }
    }
    else
    {
        savePackageMenuItem->Disable();
        saveToolButton->Disable();
        closePackageMenuItem->Disable();
        buildPackageMenuItem->Disable();
        buildToolButton->Disable();
        cancelBuildMenuItem->Disable();
        cancelBuildToolButton->Disable();
        openBinDirectoryMenuItem->Disable();
        pathBar->Hide();
    }
}

} // package_editor

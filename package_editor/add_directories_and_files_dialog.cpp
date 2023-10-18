// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.add_directories_and_files_dialog;

import package_editor.main_window;
import package_editor.configuration;
import package_editor.package;
import util;

namespace package_editor {

DirsAndFilesNode::DirsAndFilesNode() : Node(NodeKind::dirsAndFiles, std::string())
{
}

wing::Color DefaultAddDirectoriesAndFilesDialogListViewBorderColor()
{
    return wing::Color(204, 206, 219);
}

AddDirectoriesAndFilesDialog::AddDirectoriesAndFilesDialog(package_editor::Component* component_) :
    wing::Window(wing::WindowCreateParams().Text("Add Directories and Files").
        SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(100)))),
        component(component_), node()
{
    if (component->GetPackage()->GetProperties()->SourceRootDir().empty())
    {
        throw std::runtime_error("Package | Properties | source root directory is empty");
    }

    SetCaretDisabled();

    MainWindow* mainWindow = component->GetMainWindow();
    imageList = mainWindow->GetImageList();
    wing::Icon& icon = wing::Application::GetResourceManager().GetIcon("package.icon");
    SetIcon(icon);
    SetSmallIcon(icon);

    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultComboBoxSize = wing::ScreenMetrics::Get().DefaultComboBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    std::unique_ptr<wing::Panel> panelPtr(new wing::Panel(wing::ControlCreateParams().SetSize(wing::Size(0, defaultButtonSize.Height + 2 * defaultControlSpacing.Height)).
        SetDock(wing::Dock::bottom)));
    wing::Panel* panel = panelPtr.get();
    AddChild(panelPtr.release());

    std::unique_ptr<wing::ListView> listViewPtr(new wing::ListView(wing::ListViewCreateParams().AllowMultiselect(true)));
    listView = listViewPtr.get();
    listView->SetData(&node);
    listView->SetFlag(wing::ControlFlags::scrollSubject);
    listView->SetDoubleBuffered();
    listView->SetImageList(mainWindow->GetImageList());
    listView->ColumnWidthChanged().AddHandler(mainWindow, &MainWindow::ListViewColumnWidthChanged);
    int nameColumnWidthValue = 400;
    View& view = GetConfiguredViewSettings().GetView(node.ViewName());
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
    borderColor = DefaultAddDirectoriesAndFilesDialogListViewBorderColor();
    std::unique_ptr<wing::PaddedControl> paddedListViewPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(listViewPtr.release()).Defaults()));
    std::unique_ptr<wing::ScrollableControl> scrollableListViewPtr(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedListViewPtr.release()).Defaults()));
    std::unique_ptr<wing::BorderedControl> borderedListViewPtr(new wing::BorderedControl(
        wing::BorderedControlCreateParams(scrollableListViewPtr.release()).NormalSingleBorderColor(borderColor).SetDock(wing::Dock::fill)));
    AddChild(borderedListViewPtr.release());

    wing::Size panelSize = panel->GetSize();

    wing::Point labelLoc(24, 8);
    std::unique_ptr<wing::Label> sourceRootDirLabelPtr(new wing::Label(wing::LabelCreateParams().SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left)).
        Location(labelLoc).Text(util::GetFullPath(component->GetPackage()->GetProperties()->SourceRootDir()))));
    panel->AddChild(sourceRootDirLabelPtr.release());

    int x = panelSize.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = panelSize.Height - defaultButtonSize.Height - defaultControlSpacing.Height;

    wing::Point cancelButtonLoc(x, y);
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Location(cancelButtonLoc).SetSize(defaultButtonSize).Text("Cancel").
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::bottom | wing::Anchors::right))));
    wing::Button* cancelButton = cancelButtonPtr.get();
    cancelButton->SetDialogResult(wing::DialogResult::cancel);
    panel->AddChild(cancelButtonPtr.release());

    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;

    wing::Point okButtonLoc(x, y);
    std::unique_ptr<wing::Button> okButtonPtr(new wing::Button(wing::ControlCreateParams().Location(okButtonLoc).SetSize(defaultButtonSize).Text("OK").
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::bottom | wing::Anchors::right))));
    wing::Button* okButton = okButtonPtr.get();
    okButton->SetDialogResult(wing::DialogResult::ok);
    okButton->SetDefault();
    SetDefaultButton(okButton);
    SetCancelButton(cancelButton);
    panel->AddChild(okButtonPtr.release());

    AddDirectoriesAndFiles();
    AddListViewEventHandlers();
}

void AddDirectoriesAndFilesDialog::GetSelectedDirectoriesAndFiles(std::vector<std::u32string>& selectedDirectories, std::vector<std::u32string>& selectedFiles) const
{
    std::vector<wing::ListViewItem*> selectedItems = listView->GetSelectedItems();
    for (wing::ListViewItem* item : selectedItems)
    {
        DialogListViewItemData* itemData = static_cast<DialogListViewItemData*>(item->Data());
        if (itemData->kind == DialogListViewItemData::Kind::directory)
        {
            selectedDirectories.push_back(directoryNames[itemData->index]);
        }
        else if (itemData->kind == DialogListViewItemData::Kind::file)
        {
            selectedFiles.push_back(fileNames[itemData->index]);
        }
    }
}

void AddDirectoriesAndFilesDialog::AddDirectoriesAndFiles()
{
    Node* parent = component->Parent();
    if (parent && parent->Kind() == NodeKind::components)
    {
        Components* components = static_cast<Components*>(parent);
        if (components)
        {
            Package* package = component->GetPackage();
            if (package)
            {
                GetDirectoriesAndFiles(package->GetProperties()->SourceRootDir(), directoryNames, fileNames);
                directoryNames = Filter(directoryNames, component->DirectoryNames());
                fileNames = Filter(fileNames, component->FileNames());
                int nd = directoryNames.size();
                for (int i = 0; i < nd; ++i)
                {
                    const std::u32string& directoryName = directoryNames[i];
                    std::string dirName = util::ToUtf8(directoryName);
                    package_editor::Component* dirComponent = components->GetDirectoryComponent(directoryName);
                    if (dirComponent && dirComponent != component)
                    {
                        dirName.append(" (").append(dirComponent->Name()).append(")");
                    }
                    wing::ListViewItem* item = listView->AddItem();
                    item->SetImageIndex(imageList->GetImageIndex("folder.closed.bitmap"));
                    item->SetColumnValue(0, dirName);
                    DialogListViewItemData* itemData = new DialogListViewItemData(DialogListViewItemData::Kind::directory, i);
                    data.push_back(std::unique_ptr<DialogListViewItemData>(itemData));
                    item->SetData(itemData);
                }
                int nf = fileNames.size();
                for (int i = 0; i < nf; ++i)
                {
                    const std::u32string& fileName = fileNames[i];
                    std::string fName = util::ToUtf8(fileName);
                    package_editor::Component* fileComponent = components->GetFileComponent(fileName);
                    if (fileComponent && fileComponent != component)
                    {
                        fName.append(" (").append(fileComponent->Name()).append(")");
                    }
                    wing::ListViewItem* item = listView->AddItem();
                    item->SetImageIndex(imageList->GetImageIndex("file.bitmap"));
                    item->SetColumnValue(0, fName);
                    DialogListViewItemData* itemData = new DialogListViewItemData(DialogListViewItemData::Kind::file, i);
                    data.push_back(std::unique_ptr<DialogListViewItemData>(itemData));
                    item->SetData(itemData);
                }
            }
        }
    }
}

void AddDirectoriesAndFilesDialog::AddListViewEventHandlers()
{
    listView->ItemClick().AddHandler(this, &AddDirectoriesAndFilesDialog::ListViewItemClick);
    listView->ItemDoubleClick().AddHandler(this, &AddDirectoriesAndFilesDialog::ListViewItemDoubleClick);
}

void AddDirectoriesAndFilesDialog::ListViewItemClick(wing::ListViewItemEventArgs& args)
{
    if (args.item && args.view)
    {
        if (args.control)
        {
            if (args.item->IsSelected())
            {
                args.item->ResetSelected();
            }
            else
            {
                args.item->SetSelected();
            }
        }
        else
        {
            args.view->SetSelectedItem(args.item);
        }
    }
}

void AddDirectoriesAndFilesDialog::ListViewItemDoubleClick(wing::ListViewItemEventArgs& args)
{
    if (args.item && args.view)
    {
        args.view->SetSelectedItem(args.item);
    }
    SetDialogResult(wing::DialogResult::ok);
}

} // package_editor

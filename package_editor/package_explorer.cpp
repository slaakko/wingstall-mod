// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module package_editor.package_explorer;

import package_editor.package_content_view;
import package_editor.path_bar;
import package_editor.main_window;

namespace package_editor {

wing::Color DefaultPackageExplorerBackgroundColor()
{
    return wing::Color::White;
}

wing::Color DefaultPackageExplorerFrameColor()
{
    return wing::Color(204, 206, 219);
}

wing::Padding PackageExplorerNodeImagePadding()
{
    return wing::Padding(2, 2, 2, 2);
}

PackageExplorerCreateParams::PackageExplorerCreateParams()
{
    controlCreateParams.WindowClassName("package_editor.package_explorer");
    controlCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    controlCreateParams.BackgroundColor(DefaultPackageExplorerBackgroundColor());
    treeViewCreateParams = wing::TreeViewCreateParams().NodeIndentPercent(100).NodeImagePadding(PackageExplorerNodeImagePadding());
    frameColor = DefaultPackageExplorerFrameColor();
}

PackageExplorerCreateParams& PackageExplorerCreateParams::Defaults()
{
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return*this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return*this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

PackageExplorerCreateParams& PackageExplorerCreateParams::FrameColor(const wing::Color& frameColor_)
{
    frameColor = frameColor_;
    return *this;
}

PackageExplorer::PackageExplorer(PackageExplorerCreateParams& createParams) :
    wing::ContainerControl(createParams.controlCreateParams),
    mainWindow(nullptr),
    package(nullptr),
    treeView(nullptr),
    child(nullptr),
    contentView(nullptr),
    imageList(nullptr),
    treeViewCreateParams(createParams.treeViewCreateParams),
    frameColor(createParams.frameColor)
{
    MakeView();
}

void PackageExplorer::MakeView()
{
    if (child)
    {
        RemoveChild(child);
        child = nullptr;
        treeView = nullptr;
    }
    std::unique_ptr<wing::TreeView> treeViewPtr(new wing::TreeView(treeViewCreateParams));
    treeView = treeViewPtr.get();
    treeView->SetFlag(wing::ControlFlags::scrollSubject);
    treeView->SetDoubleBuffered();
    treeView->SetImageList(imageList);
    treeView->NodeClick().AddHandler(this, &PackageExplorer::TreeViewNodeClick);
    treeView->NodeDoubleClick().AddHandler(this, &PackageExplorer::TreeViewNodeDoubleClick);
    std::unique_ptr<wing::Control> paddedTreeView(new wing::PaddedControl(wing::PaddedControlCreateParams(treeViewPtr.release()).Defaults()));
    std::unique_ptr<wing::Control> borderedTreeView(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTreeView.release()).SetBorderStyle(wing::BorderStyle::single).
        NormalSingleBorderColor(frameColor)));
    std::unique_ptr<wing::Control> scrollableTreeView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(borderedTreeView.release()).SetDock(wing::Dock::fill)));
    child = scrollableTreeView.get();
    AddChild(scrollableTreeView.release());
    Invalidate();
}

void PackageExplorer::SetPackage(Package* package_)
{
    MakeView();
    package = package_;
    if (package)
    {
        treeView->SetRoot(package->ToTreeViewNode(treeView));
        package->GetTreeViewNode()->Expand();
        package->GetComponents()->GetTreeViewNode()->Expand();
    }
    Invalidate();
}

void PackageExplorer::SetImageList(wing::ImageList* imageList_)
{
    imageList = imageList_;
    if (treeView)
    {
        treeView->SetImageList(imageList);
    }
}

void PackageExplorer::SetContentView(PackageContentView* contentView_)
{
    contentView = contentView_;
}

void PackageExplorer::TreeViewNodeClick(wing::TreeViewNodeClickEventArgs& args)
{
    try
    {
        void* data = args.node->Data();
        if (data)
        {
            Node* node = static_cast<Node*>(data);
            if (args.buttons == wing::MouseButtons::lbutton)
            {
                contentView->ViewContent(node);
                node->SetCurrentPathNode();
            }
            else if (args.buttons == wing::MouseButtons::rbutton)
            {
                if (mainWindow)
                {
                    mainWindow->ClearClickActions();
                    std::unique_ptr<wing::ContextMenu> contextMenu(new wing::ContextMenu());
                    node->AddMenuItems(contextMenu.get(), mainWindow->ClickActions(), static_cast<MenuItemsKind>(MenuItemsKind::allMenuItems | MenuItemsKind::treeView));
                    if (contextMenu->HasMenuItems())
                    {
                        treeView->TranslateContentLocation(args.location);
                        wing::Point screenLoc = treeView->ClientToScreen(args.location);
                        mainWindow->ShowContextMenu(contextMenu.release(), screenLoc);
                    }
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void PackageExplorer::TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args)
{
    try
    {
        wing::TreeViewNode* treeViewNode = args.node;
        if (treeViewNode)
        {
            Node* node = nullptr;
            if (treeViewNode->Data())
            {
                node = static_cast<Node*>(treeViewNode->Data());
            }
            if (node && node->Leaf())
            {
                node->ExecuteDefaultAction();
            }
            else
            {
                if (treeViewNode->State() == wing::TreeViewNodeState::collapsed)
                {
                    treeViewNode->ExpandAll();
                }
                else if (treeViewNode->State() == wing::TreeViewNodeState::expanded)
                {
                    treeViewNode->CollapseAll();
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void PackageExplorer::OnPaint(wing::PaintEventArgs& args)
{
    try
    {
        treeView->Invalidate();
        child->Invalidate();
        Control::OnPaint(args);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void PackageExplorer::Open(Node* node)
{
    wing::TreeViewNode* treeViewNode = node->GetTreeViewNode();
    if (treeViewNode)
    {
        wing::TreeViewNode* parentNode = treeViewNode->Parent();
        while (parentNode)
        {
            parentNode->Expand();
            parentNode = parentNode->Parent();
        }
        wing::TreeView* treeView = treeViewNode->GetTreeView();
        if (treeView)
        {
            treeView->SetSelectedNode(treeViewNode);
        }
    }
}

Node* PackageExplorer::SelectedNode()
{
    if (treeView)
    {
        wing::TreeViewNode* selectedNode = treeView->SelectedNode();
        if (selectedNode)
        {
            void* data = selectedNode->Data();
            if (data)
            {
                Node* node = static_cast<Node*>(data);
                return node;
            }
        }
    }
    return nullptr;
}

} // package_editor

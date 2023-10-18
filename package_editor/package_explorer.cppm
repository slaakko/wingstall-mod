// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.package_explorer;

import package_editor.package;
import package_editor.node;
import wing;
import std.core;

export namespace package_editor {

class PackageContentView;

wing::Color DefaultPackageExplorerBackgroundColor();
wing::Color DefaultPackageExplorerFrameColor();
wing::Padding PackageExplorerNodeImagePadding();

struct PackageExplorerCreateParams
{
    PackageExplorerCreateParams();
    PackageExplorerCreateParams& Defaults();
    PackageExplorerCreateParams& WindowClassName(const std::string& windowClassName_);
    PackageExplorerCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    PackageExplorerCreateParams& WindowStyle(int windowStyle_);
    PackageExplorerCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    PackageExplorerCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    PackageExplorerCreateParams& Text(const std::string& text_);
    PackageExplorerCreateParams& Location(wing::Point location_);
    PackageExplorerCreateParams& SetSize(wing::Size size_);
    PackageExplorerCreateParams& SetAnchors(wing::Anchors anchors_);
    PackageExplorerCreateParams& SetDock(wing::Dock dock_);
    PackageExplorerCreateParams& FrameColor(const wing::Color& frameColor_);
    wing::ControlCreateParams controlCreateParams;
    wing::TreeViewCreateParams treeViewCreateParams;
    wing::Color frameColor;
};

class PackageExplorer : public wing::ContainerControl
{
public:
    PackageExplorer(PackageExplorerCreateParams& createParams);
    MainWindow* GetMainWindow() const { return mainWindow; }
    void SetMainWindow(MainWindow* mainWindow_) { mainWindow = mainWindow_; }
    void SetPackage(Package* package_);
    void SetImageList(wing::ImageList* imageList);
    void SetContentView(PackageContentView* contentView_);
    void Open(Node* node);
    Node* SelectedNode();
    void TreeViewNodeClick(wing::TreeViewNodeClickEventArgs& args);
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    void TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args);
    void MakeView();
    MainWindow* mainWindow;
    wing::TreeViewCreateParams treeViewCreateParams;
    wing::ImageList* imageList;
    wing::Color frameColor;
    Package* package;
    wing::TreeView* treeView;
    wing::Control* child;
    PackageContentView* contentView;
};

} // package_editor

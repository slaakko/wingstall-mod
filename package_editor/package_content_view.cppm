// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.package_content_view;

import package_editor.package;
import package_editor.node;
import wing;
import std.core;

export namespace package_editor {

class MainWindow;

class EmptyView : public wing::Control
{
public:
    EmptyView();
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
};

wing::Color DefaultPackageContentViewBackgroundColor();
wing::Color DefaultPackageContentViewFrameColor();

struct PackageContentViewCreateParams
{
    PackageContentViewCreateParams();
    PackageContentViewCreateParams& Defaults();
    PackageContentViewCreateParams& WindowClassName(const std::string& windowClassName_);
    PackageContentViewCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    PackageContentViewCreateParams& WindowStyle(int windowStyle_);
    PackageContentViewCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    PackageContentViewCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    PackageContentViewCreateParams& Text(const std::string& text_);
    PackageContentViewCreateParams& Location(wing::Point location_);
    PackageContentViewCreateParams& SetSize(wing::Size size_);
    PackageContentViewCreateParams& SetAnchors(wing::Anchors anchors_);
    PackageContentViewCreateParams& SetDock(wing::Dock dock_);
    PackageContentViewCreateParams& FrameColor(const wing::Color& frameColor_);
    wing::ControlCreateParams controlCreateParams;
    wing::Color frameColor;
};

class PackageContentView : public wing::ContainerControl
{
public:
    PackageContentView(PackageContentViewCreateParams& createParams);
    void SetImageList(wing::ImageList* imageList_) { imageList = imageList_; }
    void ViewContent(Node* node);
    void SetMainWindow(MainWindow* mainWindow_) { mainWindow = mainWindow_; }
    MainWindow* GetMainWindow() const { return mainWindow; }
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    wing::Control* MakeFramedControl(wing::Control* child_);
    wing::Control* framedChild;
    wing::Control* child;
    wing::Color frameColor;
    wing::ImageList* imageList;
    MainWindow* mainWindow;
};

} // package_editor

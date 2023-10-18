// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module package_editor.package_content_view;

import package_editor.main_window;

namespace package_editor {

EmptyView::EmptyView() : wing::Control(wing::ControlCreateParams().WindowClassName("package_editor.empty_view").WindowClassBackgroundColor(COLOR_WINDOW).
    BackgroundColor(wing::Color::White))
{
}

void EmptyView::OnPaint(wing::PaintEventArgs& args)
{
    args.graphics.Clear(BackgroundColor());
    Control::OnPaint(args);
}

wing::Color DefaultPackageContentViewBackgroundColor()
{
    return wing::Color::White;
}

wing::Color DefaultPackageContentViewFrameColor()
{
    return wing::Color(204, 206, 219);
}

PackageContentViewCreateParams::PackageContentViewCreateParams() : controlCreateParams()
{
    controlCreateParams.WindowClassName("package_editor.package_content_view");
    controlCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    controlCreateParams.BackgroundColor(DefaultPackageContentViewBackgroundColor());
    controlCreateParams.WindowStyle(wing::DefaultChildWindowStyle());
    frameColor = DefaultPackageContentViewFrameColor();
}

PackageContentViewCreateParams& PackageContentViewCreateParams::Defaults()
{
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

PackageContentViewCreateParams& PackageContentViewCreateParams::FrameColor(const wing::Color& frameColor_)
{
    frameColor = frameColor_;
    return*this;
}

PackageContentView::PackageContentView(PackageContentViewCreateParams& createParams) :
    wing::ContainerControl(createParams.controlCreateParams), frameColor(createParams.frameColor), imageList(nullptr), framedChild(nullptr), child(nullptr), mainWindow(nullptr)
{
    AddChild(MakeFramedControl(new EmptyView()));
}

void PackageContentView::OnPaint(wing::PaintEventArgs& args)
{
    try
    {
        if (child)
        {
            child->Invalidate();
        }
        if (framedChild)
        {
            framedChild->Invalidate();
        }
        Control::OnPaint(args);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void PackageContentView::ViewContent(Node* node)
{
    if (node)
    {
        bool cancel = false;
        wing::CancelArgs cancelArgs(cancel);
        mainWindow->ExitView().Fire(cancelArgs);
        if (cancelArgs.cancel)
        {
            return;
        }
        if (framedChild)
        {
            framedChild->Hide();
            RemoveChild(framedChild);
            framedChild = nullptr;
            child = nullptr;
        }
        node->ResetDirectoryPath();
        wing::Control* view = node->CreateView(imageList);
        if (!view)
        {
            view = new EmptyView();
        }
        std::unique_ptr<wing::Control> framedView(MakeFramedControl(view));
        AddChild(framedView.release());
    }
    else
    {
        if (framedChild)
        {
            framedChild->Hide();
            RemoveChild(framedChild);
            framedChild = nullptr;
            child = nullptr;
        }
    }
}

wing::Control* PackageContentView::MakeFramedControl(wing::Control* child_)
{
    child = child_;
    child->SetFlag(wing::ControlFlags::scrollSubject);
    wing::PaddedControl* paddedControl = new wing::PaddedControl(wing::PaddedControlCreateParams(child).Defaults());
    wing::BorderedControl* borderedControl = new wing::BorderedControl(wing::BorderedControlCreateParams(paddedControl).SetBorderStyle(wing::BorderStyle::single).
        NormalSingleBorderColor(frameColor));
    wing::ScrollableControl* scrollableControl = new wing::ScrollableControl(wing::ScrollableControlCreateParams(borderedControl).SetDock(wing::Dock::fill));
    framedChild = scrollableControl;
    return scrollableControl;
}

} // package_editor

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef min
#undef max

module package_editor.path_bar;

import util;

namespace package_editor {

wing::Color DefaultPathBarBackgroundColor()
{
    return wing::Color::White;
}

wing::Color DefaultPathBarFrameColor()
{
    return wing::Color(204, 206, 219);
}

wing::Color DefaultDirectoryPathViewBackgroundColor()
{
    return wing::Color::White;
}

wing::Color DefaultParentPathSelectorBackgroundColor()
{
    return wing::Color::White;
}

wing::Color DefaultParentPathSelectorMouseOverColor()
{
    return wing::Color(230, 243, 255);
}

wing::Color DefaultParentPathSelectorMouseClickColor()
{
    return wing::Color(204, 232, 255);
}

wing::Color DefaultPathDividerBackgroundColor()
{
    return wing::Color::White;
}

wing::Color DefaultPathDividerLineColor()
{
    return wing::Color(204, 206, 219);
}

std::string DefaultDirectoryPathViewFontFamilyName()
{
    return "Segoe UI";
}

float DefaultDirectoryPathViewFontSize()
{
    return 9.0f;
}

DirectoryPathViewCreateParams::DirectoryPathViewCreateParams()
{
    controlCreateParams.WindowClassBackgroundColor(COLOR_WINDOW).BackgroundColor(DefaultDirectoryPathViewBackgroundColor()).
        WindowClassName("package_editor.directory_path_view").SetSize(wing::Size(100, 0)).SetDock(wing::Dock::left);
    fontFamilyName = DefaultDirectoryPathViewFontFamilyName();
    fontSize = DefaultDirectoryPathViewFontSize();
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::Defaults()
{
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::FontFamilyName(const std::string& fontFamilyName_)
{
    fontFamilyName = fontFamilyName_;
    return *this;
}

DirectoryPathViewCreateParams& DirectoryPathViewCreateParams::FontSize(float fontSize_)
{
    fontSize = fontSize_;
    return *this;
}

ParentPathSelectorCreateParams::ParentPathSelectorCreateParams(wing::ImageList* imageList_) : controlCreateParams(), imageList(imageList_)
{
    controlCreateParams.BackgroundColor(DefaultParentPathSelectorBackgroundColor()).WindowClassBackgroundColor(COLOR_WINDOW).
        WindowClassName("package_editor.parent_path_selector").
        SetSize(wing::Size(20, 0)).SetDock(wing::Dock::left);
    mouseOverColor = DefaultParentPathSelectorMouseOverColor();
    mouseClickColor = DefaultParentPathSelectorMouseClickColor();
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::Defaults()
{
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::MouseOverColor(const wing::Color& mouseOverColor_)
{
    mouseOverColor = mouseOverColor_;
    return *this;
}

ParentPathSelectorCreateParams& ParentPathSelectorCreateParams::MouseClickColor(const wing::Color& mouseClickColor_)
{
    mouseClickColor = mouseClickColor_;
    return *this;
}

PathDividerCreateParams::PathDividerCreateParams()
{
    controlCreateParams.BackgroundColor(DefaultPathDividerBackgroundColor()).WindowClassBackgroundColor(COLOR_WINDOW).WindowClassName("package_editor.path_divider").
        SetSize(wing::Size(1, 0)).SetDock(wing::Dock::left);
    penColor = DefaultPathDividerLineColor();
}

PathDividerCreateParams& PathDividerCreateParams::Defaults()
{
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

PathDividerCreateParams& PathDividerCreateParams::PenColor(const wing::Color& penColor_)
{
    penColor = penColor_;
    return *this;
}

PathBarCreateParams::PathBarCreateParams(wing::ImageList* imageList_) : controlCreateParams(), imageList(imageList_), parentPathSelectorCreateParams(imageList)
{
    controlCreateParams.WindowClassName("package_editor.path_bar").WindowClassBackgroundColor(COLOR_WINDOW).BackgroundColor(DefaultPathBarBackgroundColor()).
        WindowStyle(wing::DefaultChildWindowStyle());
    frameColor = DefaultPathBarFrameColor();
}

PathBarCreateParams& PathBarCreateParams::Defaults()
{
    return *this;
}

PathBarCreateParams& PathBarCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

PathBarCreateParams& PathBarCreateParams::FrameColor(const wing::Color& frameColor_)
{
    frameColor = frameColor_;
    return *this;
}

PathBar::PathBar(PathBarCreateParams& createParams) : ContainerControl(createParams.controlCreateParams)
{
    std::unique_ptr<DirectoryPathView> directoryPathViewPtr(new DirectoryPathView(createParams.directoryPathViewCreateParams));
    directoryPathView = directoryPathViewPtr.get();
    AddChild(directoryPathViewPtr.release());
    std::unique_ptr<PathDivider> pathDividerPtr1(new PathDivider(createParams.pathDividerCreateParams));
    pathDivider1 = pathDividerPtr1.get();
    AddChild(pathDividerPtr1.release());
    std::unique_ptr<ParentPathSelector> parentPathSelectorPtr(new ParentPathSelector(createParams.parentPathSelectorCreateParams));
    parentPathSelector = parentPathSelectorPtr.get();
    AddChild(parentPathSelectorPtr.release());
    Invalidate();
    std::unique_ptr<PathDivider> pathDividerPtr2(new PathDivider(createParams.pathDividerCreateParams));
    pathDivider2 = pathDividerPtr2.get();
    AddChild(pathDividerPtr2.release());
    std::unique_ptr<wing::PathView> pathViewPtr(new wing::PathView(wing::PathViewCreateParams().SetDock(wing::Dock::left)));
    pathView = pathViewPtr.get();
    pathView->SetDoubleBuffered();
    AddChild(pathViewPtr.release());
}

void PathBar::SetDirectoryPath(const std::string& directoryPath)
{
    directoryPathView->SetDirectoryPath(directoryPath);
    Invalidate();
}

void PathBar::SetDirectoryPathViewWidth(int width)
{
    wing::Size sz = directoryPathView->GetSize();
    sz.Width = width + 4;
    directoryPathView->SetSize(sz);
    wing::Size pathDivider1Size = pathDivider1->GetSize();
    sz.Width = sz.Width + pathDivider1Size.Width;
    sz.Width = sz.Width + 32;
    wing::Size pathDivider2Size = pathDivider2->GetSize();
    sz.Width = sz.Width + pathDivider2Size.Width;
    wing::Size pvSize = pathView->GetSize();
    sz.Width = sz.Width + pvSize.Width;
    sz.Height = std::max(sz.Height, pvSize.Height);
    SetSize(sz);
}

void PathBar::SetPathViewMaxWidth(int pathViewMaxWidth)
{
    wing::Size sz = directoryPathView->GetSize();
    directoryPathView->SetSize(sz);
    wing::Size pathDivider1Size = pathDivider1->GetSize();
    sz.Width = sz.Width + pathDivider1Size.Width;
    sz.Width = sz.Width + 32;
    wing::Size pathDivider2Size = pathDivider2->GetSize();
    sz.Width = sz.Width + pathDivider2Size.Width;
    pathView->SetMaxWidth(pathViewMaxWidth);
    wing::Size pvSize = pathView->GetSize();
    sz.Width = sz.Width + pvSize.Width;
    sz.Height = std::max(sz.Height, pvSize.Height);
    SetSize(sz);
}

void PathBar::SetCurrentNode(Node* currentNode)
{
    pathView->Clear();
    while (currentNode)
    {
        pathView->PushPathComponent(currentNode->PathComponentName(), currentNode);
        currentNode = currentNode->Parent();
    }
}

void PathBar::Show()
{
    directoryPathView->Show();
    pathDivider1->Show();
    parentPathSelector->Show();
    pathDivider2->Show();
    pathView->Show();
}

void PathBar::Hide()
{
    directoryPathView->Hide();
    pathDivider1->Hide();
    parentPathSelector->Hide();
    pathDivider2->Hide();
    pathView->Hide();
}

void PathBar::OnPaint(wing::PaintEventArgs& args)
{
    if (directoryPathView)
    {
        directoryPathView->Invalidate();
    }
    if (pathDivider1)
    {
        pathDivider1->Invalidate();
    }
    if (parentPathSelector)
    {
        parentPathSelector->Invalidate();
    }
    if (pathDivider2)
    {
        pathDivider2->Invalidate();
    }
    if (pathView)
    {
        pathView->Invalidate();
    }
    Control::OnPaint(args);
}

void PathBar::OnSizeChanged()
{
    ContainerControl::OnSizeChanged();
    DockChildren();
}

DirectoryPathView::DirectoryPathView(DirectoryPathViewCreateParams& createParams) :
    Control(createParams.controlCreateParams), directoryPath(), blackBrush(wing::Color::Black)
{
    std::u16string fontFamilyName = util::ToUtf16(createParams.fontFamilyName);
    SetFont(wing::Font(wing::FontFamily((const WCHAR*)fontFamilyName.c_str()), createParams.fontSize, wing::FontStyle::FontStyleRegular, wing::Unit::UnitPoint));
}

void DirectoryPathView::SetDirectoryPath(const std::string& directoryPath_)
{
    directoryPath = directoryPath_;
    Invalidate();
}

void DirectoryPathView::OnPaint(wing::PaintEventArgs& args)
{
    args.graphics.Clear(BackgroundColor());
    wing::PointF origin(4, 6);
    wing::DrawString(args.graphics, directoryPath, GetFont(), origin, blackBrush);
}

ParentPathSelector::ParentPathSelector(ParentPathSelectorCreateParams& createParams) :
    wing::Control(createParams.controlCreateParams),
    state(ParentPathSelectorState::idle),
    mouseOverBrush(createParams.mouseOverColor),
    mouseClickBrush(createParams.mouseClickColor)
{
    imageList = createParams.imageList;
}

void ParentPathSelector::OnPaint(wing::PaintEventArgs& args)
{
    args.graphics.Clear(BackgroundColor());
    if (state == ParentPathSelectorState::mouseOver)
    {
        wing::Point loc(0, 0);
        wing::Rect rect(loc, GetSize());
        args.graphics.FillRectangle(&mouseOverBrush, rect);
    }
    else if (state == ParentPathSelectorState::clicked)
    {
        wing::Point loc(0, 0);
        wing::Rect rect(loc, GetSize());
        args.graphics.FillRectangle(&mouseClickBrush, rect);
    }
    wing::Point loc;
    wing::Size sz(GetSize());
    wing::Rect rect(loc, sz);
    if (sz.Width > 0 && sz.Height > 0)
    {
        wing::Bitmap* bitmap = imageList->GetImage(imageList->GetImageIndex("up.arrow.bitmap"));
        wing::Padding padding(2, 7, 2, 7);
        wing::PointF imageLoc(padding.left, padding.top);
        wing::RectF r(imageLoc, wing::SizeF(bitmap->GetWidth() + padding.Horizontal(), bitmap->GetHeight() + padding.Vertical()));
        Gdiplus::ImageAttributes attributes;
        attributes.SetColorKey(wing::DefaultBitmapTransparentColor(), wing::DefaultBitmapTransparentColor());
        wing::CheckGraphicsStatus(args.graphics.DrawImage(bitmap, r, 0, 0, bitmap->GetWidth() + padding.Horizontal(), bitmap->GetHeight() + padding.Vertical(), wing::Unit::UnitPixel, &attributes));
    }
}

void ParentPathSelector::SetState(ParentPathSelectorState state_)
{
    if (state != state_)
    {
        state = state_;
        Invalidate();
    }
}

void ParentPathSelector::OnMouseEnter()
{
    Control::OnMouseEnter();
    SetState(ParentPathSelectorState::mouseOver);
}

void ParentPathSelector::OnMouseLeave()
{
    Control::OnMouseLeave();
    SetState(ParentPathSelectorState::idle);
}

void ParentPathSelector::OnMouseDown(wing::MouseEventArgs& args)
{
    Control::OnMouseDown(args);
    SetState(ParentPathSelectorState::clicked);
}

void ParentPathSelector::OnMouseUp(wing::MouseEventArgs& args)
{
    Control::OnMouseUp(args);
    SetState(ParentPathSelectorState::mouseOver);
}

PathDivider::PathDivider(PathDividerCreateParams& createParams) :
    Control(createParams.controlCreateParams), pen(createParams.penColor)
{
}

void PathDivider::OnPaint(wing::PaintEventArgs& args)
{
    wing::PointF start(0, 0);
    wing::Size sz = GetSize();
    wing::PointF end(0, sz.Height);
    args.graphics.DrawLine(&pen, start, end);
}

} // package_editor

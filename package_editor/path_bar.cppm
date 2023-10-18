// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.path_bar;

import package_editor.node;
import wing;
import std.core;

export namespace package_editor {

class DirectoryPathView;
class ParentPathSelector;
class PathDivider;

wing::Color DefaultPathBarBackgroundColor();
wing::Color DefaultPathBarFrameColor();
wing::Color DefaultDirectoryPathViewBackgroundColor();
wing::Color DefaultParentPathSelectorBackgroundColor();
wing::Color DefaultParentPathSelectorMouseOverColor();
wing::Color DefaultParentPathSelectorMouseClickColor();
wing::Color DefaultPathDividerBackgroundColor();
wing::Color DefaultPathDividerLineColor();
std::string DefaultDirectoryPathViewFontFamilyName();
float DefaultDirectoryPathViewFontSize();

struct DirectoryPathViewCreateParams
{
    DirectoryPathViewCreateParams();
    DirectoryPathViewCreateParams& Defaults();
    DirectoryPathViewCreateParams& WindowClassName(const std::string& windowClassName_);
    DirectoryPathViewCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    DirectoryPathViewCreateParams& WindowStyle(int windowStyle_);
    DirectoryPathViewCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    DirectoryPathViewCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    DirectoryPathViewCreateParams& Text(const std::string& text_);
    DirectoryPathViewCreateParams& Location(wing::Point location_);
    DirectoryPathViewCreateParams& SetSize(wing::Size size_);
    DirectoryPathViewCreateParams& SetAnchors(wing::Anchors anchors_);
    DirectoryPathViewCreateParams& SetDock(wing::Dock dock_);
    DirectoryPathViewCreateParams& FontFamilyName(const std::string& fontFamilyName_);
    DirectoryPathViewCreateParams& FontSize(float fontSize_);
    wing::ControlCreateParams controlCreateParams;
    std::string fontFamilyName;
    float fontSize;
};

struct ParentPathSelectorCreateParams
{
    ParentPathSelectorCreateParams(wing::ImageList* imageList_);
    ParentPathSelectorCreateParams& Defaults();
    ParentPathSelectorCreateParams& WindowClassName(const std::string& windowClassName_);
    ParentPathSelectorCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    ParentPathSelectorCreateParams& WindowStyle(int windowStyle_);
    ParentPathSelectorCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    ParentPathSelectorCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    ParentPathSelectorCreateParams& Text(const std::string& text_);
    ParentPathSelectorCreateParams& Location(wing::Point location_);
    ParentPathSelectorCreateParams& SetSize(wing::Size size_);
    ParentPathSelectorCreateParams& SetAnchors(wing::Anchors anchors_);
    ParentPathSelectorCreateParams& SetDock(wing::Dock dock_);
    ParentPathSelectorCreateParams& MouseOverColor(const wing::Color& mouseOverColor_);
    ParentPathSelectorCreateParams& MouseClickColor(const wing::Color& mouseClickColor_);
    wing::ControlCreateParams controlCreateParams;
    wing::ImageList* imageList;
    wing::Color mouseOverColor;
    wing::Color mouseClickColor;
};

struct PathDividerCreateParams
{
    PathDividerCreateParams();
    PathDividerCreateParams& Defaults();
    PathDividerCreateParams& WindowClassName(const std::string& windowClassName_);
    PathDividerCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    PathDividerCreateParams& WindowStyle(int windowStyle_);
    PathDividerCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    PathDividerCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    PathDividerCreateParams& Text(const std::string& text_);
    PathDividerCreateParams& Location(wing::Point location_);
    PathDividerCreateParams& SetSize(wing::Size size_);
    PathDividerCreateParams& SetAnchors(wing::Anchors anchors_);
    PathDividerCreateParams& SetDock(wing::Dock dock_);
    PathDividerCreateParams& PenColor(const wing::Color& penColor_);
    wing::ControlCreateParams controlCreateParams;
    wing::Color penColor;
};

struct PathBarCreateParams
{
    PathBarCreateParams(wing::ImageList* imageList_);
    PathBarCreateParams& Defaults();
    PathBarCreateParams& WindowClassName(const std::string& windowClassName_);
    PathBarCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    PathBarCreateParams& WindowStyle(int windowStyle_);
    PathBarCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    PathBarCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    PathBarCreateParams& Text(const std::string& text_);
    PathBarCreateParams& Location(wing::Point location_);
    PathBarCreateParams& SetSize(wing::Size size_);
    PathBarCreateParams& SetAnchors(wing::Anchors anchors_);
    PathBarCreateParams& SetDock(wing::Dock dock_);
    PathBarCreateParams& FrameColor(const wing::Color& frameColor_);
    wing::ControlCreateParams controlCreateParams;
    wing::ImageList* imageList;
    DirectoryPathViewCreateParams directoryPathViewCreateParams;
    ParentPathSelectorCreateParams parentPathSelectorCreateParams;
    PathDividerCreateParams pathDividerCreateParams;
    wing::Color frameColor;
};

class PathBar : public wing::ContainerControl
{
public:
    PathBar(PathBarCreateParams& createParams);
    void SetDirectoryPath(const std::string& directoryPath);
    void SetDirectoryPathViewWidth(int width);
    void SetPathViewMaxWidth(int pathViewMaxWidth);
    ParentPathSelector* GetParentPathSelector() const { return parentPathSelector; }
    wing::PathView* GetPathView() const { return pathView; }
    void SetCurrentNode(Node* currentNode);
    void Show();
    void Hide();
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
    void OnSizeChanged() override;
private:
    DirectoryPathView* directoryPathView;
    PathDivider* pathDivider1;
    ParentPathSelector* parentPathSelector;
    PathDivider* pathDivider2;
    wing::PathView* pathView;
};

class DirectoryPathView : public wing::Control
{
public:
    DirectoryPathView(DirectoryPathViewCreateParams& createParams);
    void SetDirectoryPath(const std::string& directoryPath_);
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    std::string directoryPath;
    wing::SolidBrush blackBrush;
};

enum class ParentPathSelectorState : int
{
    idle, mouseOver, clicked
};

class ParentPathSelector : public wing::Control
{
public:
    ParentPathSelector(ParentPathSelectorCreateParams& createParams);
    void SetState(ParentPathSelectorState state_);
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
    void OnMouseEnter() override;
    void OnMouseLeave() override;
    void OnMouseDown(wing::MouseEventArgs& args) override;
    void OnMouseUp(wing::MouseEventArgs& args) override;
private:
    wing::ImageList* imageList;
    ParentPathSelectorState state;
    wing::SolidBrush mouseOverBrush;
    wing::SolidBrush mouseClickBrush;
};

class PathDivider : public wing::Control
{
public:
    PathDivider(PathDividerCreateParams& createParams);
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    wing::Pen pen;
};

} // package_editor

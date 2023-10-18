// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.bordered_control;

import wing.control;
import wing.container;
import wing.graphics;
import wing.base;
import std.core;

export namespace wing {

enum class BorderStyle : int
{
    single, threeD
};

int BorderWidth(BorderStyle borderStyle);
Size BorderedSize(const Size& size, BorderStyle borderStyle);
Color DefaultNormalSingleBorderColor();
Color DefaultFocusedSingleBorderColor();
Color Default3DOuterLeftTopEdgeColor();
Color Default3DOuterRightBottomEdgeColor();
Color Default3DInnerLeftTopEdgeColor();
Color Default3DInnerRightBottomEdgeColor();

enum class BorderedControlFlags : int
{
    none = 0, childFocused = 1 << 0
};

inline BorderedControlFlags operator&(BorderedControlFlags left, BorderedControlFlags right)
{
    return BorderedControlFlags(int(left) & int(right));
}

inline BorderedControlFlags operator|(BorderedControlFlags left, BorderedControlFlags right)
{
    return BorderedControlFlags(int(left) | int(right));
}

inline BorderedControlFlags operator~(BorderedControlFlags flags)
{
    return BorderedControlFlags(~int(flags));
}

struct BorderedControlCreateParams
{
    BorderedControlCreateParams(Control* child_);
    BorderedControlCreateParams& WindowClassName(const std::string& windowClassName_);
    BorderedControlCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    BorderedControlCreateParams& WindowStyle(int windowStyle_);
    BorderedControlCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    BorderedControlCreateParams& BackgroundColor(const Color& backgroundColor_);
    BorderedControlCreateParams& Text(const std::string& text_);
    BorderedControlCreateParams& Location(const Point& location_);
    BorderedControlCreateParams& SetSize(Size size_);
    BorderedControlCreateParams& SetAnchors(Anchors anchors_);
    BorderedControlCreateParams& SetDock(Dock dock_);
    BorderedControlCreateParams& SetBorderStyle(BorderStyle borderStyle_);
    BorderedControlCreateParams& NormalSingleBorderColor(const Color& color);
    BorderedControlCreateParams& FocusedSingleBorderColor(const Color& color);
    BorderedControlCreateParams& ThreeDOuterLeftTopEdgeColor(const Color& color);
    BorderedControlCreateParams& ThreeDOuterBottomRightEdgeColor(const Color& color);
    BorderedControlCreateParams& ThreeDInnerLeftTopEdgeColor(const Color& color);
    BorderedControlCreateParams& ThreeDInnerRightBottomEdgeColor(const Color& color);
    ControlCreateParams controlCreateParams;
    Control* child;
    BorderStyle borderStyle;
    Color normalSingleBorderColor;
    Color focusedSingleBorderColor;
    Color threeDOuterLeftTopEdgeColor;
    Color threeDOuterRightBottomEdgeColor;
    Color threeDInnerLeftTopEdgeColor;
    Color threeDInnerRightBottomEdgeColor;
};

class BorderedControl : public Control
{
public:
    BorderedControl(BorderedControlCreateParams& createParams);
    bool IsDecoratorControl() const override { return true; }
    Control* Child() const { return child; }
    ContainerControl* GetContainerControl() const override;
    Control* GetFirstEnabledTabStopControl() const override;
    Control* GetLastEnabledTabStopControl() const override;
protected:
    void OnPaint(PaintEventArgs& args) override;
    void OnLocationChanged() override;
    void OnSizeChanged() override;
    void OnChildSizeChanged(ControlEventArgs& args) override;
    void OnChildContentChanged(ControlEventArgs& args) override;
    void OnChildContentLocationChanged(ControlEventArgs& args) override;
    void OnChildContentSizeChanged(ControlEventArgs& args) override;
    void OnChildGotFocus(ControlEventArgs& args) override;
    void OnChildLostFocus(ControlEventArgs& args) override;
private:
    void SetChildPos();
    void DrawSingleBorder(Graphics& graphics);
    void Draw3DBorder(Graphics& graphics);
    BorderedControlFlags flags;
    Control* child;
    Container container;
    BorderStyle borderStyle;
    Color normalSingleBorderColor;
    Color focusedSingleBorderColor;
    Color threeDOuterLeftTopEdgeColor;
    Color threeDOuterRightBottomEdgeColor;
    Color threeDInnerLeftTopEdgeColor;
    Color threeDInnerRightBottomEdgeColor;
};

} // wing

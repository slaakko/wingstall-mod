// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.tool_tip;

import wing.control;
import wing.graphics;
import std.core;

export namespace wing {

enum class ToolTipFlags : int
{
    none = 0, changed = 1 << 0
};

inline ToolTipFlags operator|(ToolTipFlags left, ToolTipFlags right)
{
    return ToolTipFlags(int(left) | int(right));
}

inline ToolTipFlags operator&(ToolTipFlags left, ToolTipFlags right)
{
    return ToolTipFlags(int(left) & int(right));
}

inline ToolTipFlags operator~(ToolTipFlags flags)
{
    return ToolTipFlags(~int(flags));
}

struct ToolTipCreateParams
{
    ToolTipCreateParams();
    ToolTipCreateParams& Defaults();
    ToolTipCreateParams& WindowClassName(const std::string& windowClassName_);
    ToolTipCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    ToolTipCreateParams& WindowStyle(int windowStyle_);
    ToolTipCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    ToolTipCreateParams& BackgroundColor(const Color& backgroundColor_);
    ToolTipCreateParams& Text(const std::string& text_);
    ToolTipCreateParams& Location(Point location_);
    ToolTipCreateParams& SetSize(Size size_);
    ToolTipCreateParams& SetAnchors(Anchors anchors_);
    ToolTipCreateParams& SetDock(Dock dock_);
    ToolTipCreateParams& TextColor(const Color& color);
    ToolTipCreateParams& FontFamilyName(const std::string& fontFamilyName_);
    ToolTipCreateParams& FontSize(float size);
    ControlCreateParams controlCreateParams;
    Color textColor;
    Color frameColor;
    std::string fontFamilyName;
    float fontSize;
};

class ToolTip : public Control
{
public:
    ToolTip(ToolTipCreateParams& createParams);
    void MeasureExtent();
protected:
    void OnTextChanged() override;
    void OnPaint(PaintEventArgs& args) override;
private:
    bool Changed() const { return (flags & ToolTipFlags::changed) != ToolTipFlags::none; }
    void SetChanged() { flags = flags | ToolTipFlags::changed; }
    void ResetChanged() { flags = flags & ~ToolTipFlags::changed; }
    void Measure(Graphics& graphics);
    ToolTipFlags flags;
    Color textColor;
    Color frameColor;
    Pen framePen;
    SolidBrush textBrush;
    std::vector<std::string> lines;
    float textHeight;
    StringFormat stringFormat;
};

} // wing

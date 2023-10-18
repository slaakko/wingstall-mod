// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.text_box;

import wing.control;
import wing.graphics;
import std.core;

export namespace wing {

struct TextBoxCreateParams
{
    TextBoxCreateParams();
    TextBoxCreateParams& Defaults();
    TextBoxCreateParams& WindowClassName(const std::string& windowClassName_);
    TextBoxCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    TextBoxCreateParams& WindowStyle(int windowStyle_);
    TextBoxCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    TextBoxCreateParams& BackgroundColor(const Color& backgroundColor_);
    TextBoxCreateParams& Text(const std::string& text_);
    TextBoxCreateParams& Location(Point location_);
    TextBoxCreateParams& SetSize(Size size_);
    TextBoxCreateParams& SetAnchors(Anchors anchors_);
    TextBoxCreateParams& SetDock(Dock dock_);
    TextBoxCreateParams& SelectAll(bool selectAll_);
    TextBoxCreateParams& Multiline(bool multiline_);
    ControlCreateParams controlCreateParams;
    bool selectAll;
    bool multiline;
};

enum class TextBoxFlags : int
{
    none = 0, selectAll = 1 << 0, multiline = 1 << 1
};

inline TextBoxFlags operator|(TextBoxFlags left, TextBoxFlags right)
{
    return TextBoxFlags(int(left) | int(right));
}

inline TextBoxFlags operator&(TextBoxFlags left, TextBoxFlags right)
{
    return TextBoxFlags(int(left) & int(right));
}

inline TextBoxFlags operator~(TextBoxFlags flags)
{
    return TextBoxFlags(~int(flags));
}

class TextBox : public Control
{
public:
    TextBox(TextBoxCreateParams& createParams);
    bool IsTextBox() const override { return true; }
    void Clear();
    void SelectAll();
    bool IsMultiline() const { return (flags & TextBoxFlags::multiline) != TextBoxFlags::none; }
    void SetMultiline() { flags = flags | TextBoxFlags::multiline; }
    void ResetMultiline() { flags = flags & ~TextBoxFlags::multiline; }
protected:
    void OnCreated() override;
    void OnGotFocus();
private:
    TextBoxFlags flags;
};

} // wing

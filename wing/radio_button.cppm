// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.radio_button;

import wing.button;
import wing.control;
import wing.event;
import wing.graphics;
import std.core;

export namespace wing {

using CheckedChangedEvent = Event;

struct RadioButtonCreateParams
{
    RadioButtonCreateParams();
    RadioButtonCreateParams& Defaults();
    RadioButtonCreateParams& WindowClassName(const std::string& windowClassName_);
    RadioButtonCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    RadioButtonCreateParams& WindowStyle(int windowStyle_);
    RadioButtonCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    RadioButtonCreateParams& BackgroundColor(const Color& backgroundColor_);
    RadioButtonCreateParams& Text(const std::string& text_);
    RadioButtonCreateParams& Location(Point location_);
    RadioButtonCreateParams& SetSize(Size size_);
    RadioButtonCreateParams& SetAnchors(Anchors anchors_);
    RadioButtonCreateParams& SetDock(Dock dock_);
    RadioButtonCreateParams& AutoSize(bool autoSize_);
    ControlCreateParams controlCreateParams;
    bool autoSize;
};

enum class RadioButtonFlags : int
{
    none = 0, checked = 1 << 0, autoSize = 1 << 1, autoSized = 1 << 2
};

inline RadioButtonFlags operator|(RadioButtonFlags left, RadioButtonFlags right)
{
    return RadioButtonFlags(int(left) | int(right));
}

inline RadioButtonFlags operator&(RadioButtonFlags left, RadioButtonFlags right)
{
    return RadioButtonFlags(int(left) & int(right));
}

inline RadioButtonFlags operator~(RadioButtonFlags flags)
{
    return RadioButtonFlags(~int(flags));
}

class RadioButton : public ButtonBase
{
public:
    RadioButton(RadioButtonCreateParams& createParams);
    bool IsRadioButton() const override { return true; }
    bool Checked() const { return (flags & RadioButtonFlags::checked) != RadioButtonFlags::none; }
    void SetChecked(bool checked);
    CheckedChangedEvent& CheckedChanged() { return checkedChanged; }
protected:
    void OnCreated() override;
    void OnClick() override;
    void OnTextChanged() override;
    virtual void OnCheckedChanged();
private:
    void SetCheckedFlag();
    void ResetCheckedFlag();
    void DoAutoSize();
    Size GetButtonSize();
    Size GetBorderSize();
    RadioButtonFlags flags;
    CheckedChangedEvent checkedChanged;
};

} // wing

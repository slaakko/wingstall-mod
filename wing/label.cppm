// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.label;

import wing.control;
import wing.base;
import wing.graphics;
import std.core;

export namespace wing {

struct LabelCreateParams
{
    LabelCreateParams();
    LabelCreateParams& Defaults();
    LabelCreateParams& WindowClassName(const std::string& windowClassName_);
    LabelCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    LabelCreateParams& WindowStyle(int windowStyle_);
    LabelCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    LabelCreateParams& BackgroundColor(const Color& backgroundColor_);
    LabelCreateParams& Text(const std::string& text_);
    LabelCreateParams& Location(Point location_);
    LabelCreateParams& SetSize(Size size_);
    LabelCreateParams& SetAnchors(Anchors anchors_);
    LabelCreateParams& SetDock(Dock dock_);
    LabelCreateParams& AutoSize(bool autoSize_);
    ControlCreateParams controlCreateParams;
    bool autoSize;
};

enum class LabelFlags : int
{
    none = 0, autoSize = 1 << 0, autoSized = 1 << 1
};

inline LabelFlags operator|(LabelFlags left, LabelFlags right)
{
    return LabelFlags(int(left) | int(right));
}

inline LabelFlags operator&(LabelFlags left, LabelFlags right)
{
    return LabelFlags(int(left) & int(right));
}

inline LabelFlags operator~(LabelFlags flags)
{
    return LabelFlags(~int(flags));
}

class Label : public Control
{
public:
    Label(LabelCreateParams& createParams);
protected:
    void OnCreated() override;
    void OnTextChanged() override;
private:
    void DoAutoSize();
    LabelFlags flags;
};

} // wing

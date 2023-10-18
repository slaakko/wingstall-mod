// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.padded_control;

import wing.base;
import wing.control;
import wing.container;
import wing.graphics;
import std.core;

export namespace wing {

const int defaultControlPadding = 4;

Padding DefaultPadding();

Size PaddedSize(const Size& size, const Padding& padding);

struct PaddedControlCreateParams
{
    PaddedControlCreateParams(Control* child_);
    PaddedControlCreateParams& Defaults();
    PaddedControlCreateParams& WindowClassName(const std::string& windowClassName_);
    PaddedControlCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    PaddedControlCreateParams& WindowStyle(int windowStyle_);
    PaddedControlCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    PaddedControlCreateParams& BackgroundColor(const Color& backgroundColor_);
    PaddedControlCreateParams& Text(const std::string& text_);
    PaddedControlCreateParams& Location(const Point& location_);
    PaddedControlCreateParams& SetSize(Size size_);
    PaddedControlCreateParams& SetAnchors(Anchors anchors_);
    PaddedControlCreateParams& SetDock(Dock dock_);
    PaddedControlCreateParams& SetPadding(const Padding& padding_);
    ControlCreateParams controlCreateParams;
    Control* child;
    Padding padding;
};

class PaddedControl : public Control
{
public:
    PaddedControl(PaddedControlCreateParams& createParams);
    Control* Child() const { return child; }
    bool IsDecoratorControl() const override { return true; }
    const Padding& GetPadding() const { return padding; }
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
    Control* child;
    Container container;
    Padding padding;
};

} // wing

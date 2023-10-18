// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.button;

import wing.control;
import wing.window;

export namespace wing {

class ButtonBase : public Control
{
public:
    ButtonBase(ControlCreateParams& createParams);
    void DoClick();
};

class Button : public ButtonBase
{
public:
    Button(ControlCreateParams& createParams);
    bool IsButton() const override { return true; }
    bool IsDefault() const { return isDefault; }
    void SetDefault();
    void ResetDefault();
    DialogResult GetDialogResult() const { return dialogResult; }
    void SetDialogResult(DialogResult dialogResult_) { dialogResult = dialogResult_; }
protected:
    void OnClick() override;
    void OnCreated() override;
    void OnGotFocus() override;
    void OnLostFocus() override;
private:
    bool isDefault;
    DialogResult dialogResult;
    void SetDefaultButtonStyle();
    void ResetDefaultButtonStyle();
};

} // wing

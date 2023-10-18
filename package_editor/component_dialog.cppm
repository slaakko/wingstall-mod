// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.component_dialog;

import wing;
import std.core;

export namespace package_editor {

class ComponentDialog : public wing::Window
{
public:
    ComponentDialog(const std::string& caption_);
    std::string ComponentName() const { return textBox->Text(); }
    void SetComponentName(const std::string& componentName);
private:
    void TextBoxTextChanged();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::TextBox* textBox;
};

} // package_editor

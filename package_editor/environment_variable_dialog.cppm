// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.environment_variable_dialog;

import package_editor.environment;
import wing;
import std.core;

export namespace package_editor {

class EnvironmentVariableDialog : public wing::Window
{
public:
    EnvironmentVariableDialog(const std::string& caption);
    const std::string& EnvironmentVariableName() const { return nameTextBox->Text(); }
    void SetData(EnvironmentVariable* environmentVariable);
    void GetData(EnvironmentVariable* environmentVariable);
private:
    void CheckValid();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::TextBox* nameTextBox;
    wing::TextBox* valueTextBox;
};

} // package_editor

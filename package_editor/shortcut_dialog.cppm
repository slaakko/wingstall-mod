// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.shortcut_dialog;

import package_editor.links;
import wing;
import std.core;

export namespace package_editor {

class ShortcutDialog : public wing::Window
{
public:
    ShortcutDialog(const std::string& caption);
    const std::string& LinkFilePath() const { return linkFilePathTextBox->Text(); }
    void GetData(Shortcut* shortcut);
    void SetData(Shortcut* shortcut);
private:
    void CheckValid();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::TextBox* linkFilePathTextBox;
    wing::TextBox* pathTextBox;
    wing::TextBox* argumentsTextBox;
    wing::TextBox* workingDirectoryTextBox;
    wing::TextBox* descriptionTextBox;
    wing::TextBox* iconFilePathTextBox;
    wing::TextBox* iconIndexTextBox;
};

} // package_editor

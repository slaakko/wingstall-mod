// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.new_package_dialog;

import wing;
import std.core;

export namespace package_editor {

class NewPackageDialog : public wing::Window
{
public:
    NewPackageDialog();
    std::string GetPackageFilePath() const;
    std::string GetPackageName() const;
private:
    void CheckInput();
    void SelectPackageLocation();
    wing::Button* okButton;
    wing::TextBox* locationTextBox;
    wing::TextBox* packageNameTextBox;
};

} // package_editor

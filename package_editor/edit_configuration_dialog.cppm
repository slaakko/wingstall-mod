// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.edit_configuration_dialog;

import wing;
import std.core;

export namespace package_editor {

class EditConfigurationDialog : public wing::Window
{
public:
    EditConfigurationDialog(const std::string& boostIncludeDir, const std::string& boostLibDir, const std::string& vcVarsPath, const std::string& vcPlatformToolset);
    void GetData(std::string& boostIncludeDir, std::string& boostLibDir, std::string& vcVarsPath, std::string& vcPlatformToolset);
private:
    void CheckInput();
    void SelectBoostIncludeDir();
    void SelectBoostLibDir();
    void SelectVCVarsPath();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::TextBox* boostIncludeDirTextBox;
    wing::TextBox* boostLibDirTextBox;
    wing::TextBox* vcVarsPathTextBox;
    wing::TextBox* vcPlatformToolsetTextBox;
};

} // package_editor

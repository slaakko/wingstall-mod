// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.path_directory_dialog;

import package_editor.environment;
import wing;
import std.core;

export namespace package_editor {

class PathDirectoryDialog : public wing::Window
{
public:
    PathDirectoryDialog(const std::string& caption);
    const std::string& PathDirectoryValue() const { return pathTextBox->Text(); }
    void SetData(PathDirectory* pathDirectory);
    void GetData(PathDirectory* pathDirectory);
private:
    void CheckValid();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::TextBox* pathTextBox;
};

} // package_editor

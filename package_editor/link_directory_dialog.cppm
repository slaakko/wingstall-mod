// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.link_directory_dialog;

import package_editor.links;
import wing;
import std.core;

export namespace package_editor {

class LinkDirectoryDialog : public wing::Window
{
public:
    LinkDirectoryDialog(const std::string& caption);
    const std::string& PathDirectoryValue() const { return pathTextBox->Text(); }
    void SetData(LinkDirectory* linkDirectory);
    void GetData(LinkDirectory* linkDirectory);
private:
    void CheckValid();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::TextBox* pathTextBox;
};

} // package_editor

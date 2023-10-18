// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.add_directories_and_files_dialog;

import package_editor.component;
import package_editor.node;
import wing;
import std.core;

export namespace package_editor {

wing::Color DefaultAddDirectoriesAndFilesDialogListViewBorderColor();

struct DialogListViewItemData
{
    enum Kind
    {
        directory, file
    };
    DialogListViewItemData(Kind kind_, int index_) : kind(kind_), index(index_) {}
    Kind kind;
    int index;
};

class DirsAndFilesNode : public Node
{
public:
    DirsAndFilesNode();
    std::string ViewName() const { return "dirs_and_files_view"; }
};

class AddDirectoriesAndFilesDialog : public wing::Window
{
public:
    AddDirectoriesAndFilesDialog(package_editor::Component* component_);
    void GetSelectedDirectoriesAndFiles(std::vector<std::u32string>& selectedDirectories, std::vector<std::u32string>& selectedFiles) const;
private:
    void AddDirectoriesAndFiles();
    void AddListViewEventHandlers();
    void ListViewItemClick(wing::ListViewItemEventArgs& args);
    void ListViewItemDoubleClick(wing::ListViewItemEventArgs& args);
    package_editor::Component* component;
    std::vector<std::u32string> directoryNames;
    std::vector<std::u32string> fileNames;
    std::vector<std::unique_ptr<DialogListViewItemData>> data;
    wing::ImageList* imageList;
    wing::ListView* listView;
    wing::Color borderColor;
    DirsAndFilesNode node;
};

} // package_editor

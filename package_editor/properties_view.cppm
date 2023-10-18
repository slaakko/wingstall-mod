// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.properties_view;

import wing;
import std.core;

export namespace package_editor {

bool IsValidPath(const std::string& text);

class Package;
class MainWindow;

class PropertiesView : public wing::ContainerControl
{
public:
    PropertiesView(Package* package_);
    ~PropertiesView();
protected:
    void OnCreated() override;
    void OnSizeChanged() override;
private:
    void Exit(wing::CancelArgs& args);
    void EnableApply();
    bool CheckApplyChanges();
    void ApplyButtonClick();
    void SelectSourceRootDir();
    void CreateProductId();
    void SourceRootDirRelativeCheckBoxCheckedChanged();
    void KeyPreview(wing::Keys keys, bool& handled);
    bool initializing;
    Package* package;
    MainWindow* mainWindow;
    int exitHandlerId;
    wing::Size defaultButtonSize;
    wing::Size defaultControlSpacing;
    wing::ComboBox* compressionComboBox;
    wing::CheckBox* includeUninstallerCheckBox;
    wing::CheckBox* sourceRootDirRelativeCheckBox;
    wing::Button* applyButton;
    wing::TextBox* sourceRootDirTextBox;
    wing::TextBox* targetRootDirTextBox;
    wing::TextBox* appNameTextBox;
    wing::TextBox* publisherTextBox;
    wing::TextBox* versionTextBox;
    wing::TextBox* productIdTextBox;
    wing::TextBox* iconFilePathTextBox;
    bool dirty;
};

} // package_editor

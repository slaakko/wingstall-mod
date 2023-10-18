// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.rule_dialog;

import package_editor.rule;
import wing;
import std.core;

export namespace package_editor {

class RuleDialog : public wing::Window
{
public:
    RuleDialog(const std::string& caption);
    void SetData(Rule* rule);
    void GetData(Rule* rule);
private:
    void CheckValid();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::ComboBox* ruleKindComboBox;
    wing::ComboBox* pathKindComboBox;
    wing::TextBox* textBox;
    wing::CheckBox* cascadeCheckBox;
};

} // package_editor

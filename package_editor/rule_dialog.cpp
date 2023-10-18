// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.rule_dialog;

namespace package_editor {

RuleDialog::RuleDialog(const std::string& caption) : wing::Window(wing::WindowCreateParams().Text(caption).WindowClassName("package_editor.rule_dialog").
    WindowStyle(wing::DialogWindowStyle()).WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), wing::ScreenMetrics::Get().MMToVerticalPixels(80)))),
    okButton(nullptr),
    cancelButton(nullptr),
    ruleKindComboBox(nullptr),
    pathKindComboBox(nullptr),
    cascadeCheckBox(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();
    wing::Size defaultComboBoxSize = wing::ScreenMetrics::Get().DefaultComboBoxSize();

    wing::Point ruleKindComboBoxLoc(16, 16);
    wing::Size ruleKindComboBoxSize(defaultComboBoxSize.Width, defaultComboBoxSize.Height + 48);
    std::unique_ptr<wing::ComboBox> ruleKindComboBoxPtr(new wing::ComboBox(wing::ComboBoxCreateParams().Location(ruleKindComboBoxLoc).SetSize(ruleKindComboBoxSize).SetAnchors(
        static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    ruleKindComboBox = ruleKindComboBoxPtr.get();
    AddChild(ruleKindComboBoxPtr.release());
    ruleKindComboBox->AddItem("Exclude");
    ruleKindComboBox->AddItem("Include");
    ruleKindComboBox->SetSelectedIndex(0);
    ruleKindComboBox->SelectedIndexChanged().AddHandler(this, &RuleDialog::CheckValid);

    wing::Point pathKindComboBoxLoc(16 + defaultComboBoxSize.Width + defaultControlSpacing.Width, 16);
    wing::Size pathKindComboBoxSize(defaultComboBoxSize.Width, defaultComboBoxSize.Height + 48);
    std::unique_ptr<wing::ComboBox> pathKindComboBoxPtr(new wing::ComboBox(wing::ComboBoxCreateParams().Location(pathKindComboBoxLoc).SetSize(pathKindComboBoxSize).SetAnchors(
        static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    pathKindComboBox = pathKindComboBoxPtr.get();
    AddChild(pathKindComboBoxPtr.release());
    pathKindComboBox->AddItem("Directory");
    pathKindComboBox->AddItem("File");
    pathKindComboBox->SetSelectedIndex(0);
    pathKindComboBox->SelectedIndexChanged().AddHandler(this, &RuleDialog::CheckValid);

    wing::Point nameLabelLoc(16, 24 + 24);
    std::unique_ptr<wing::Label> nameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Name/Pattern:").Location(nameLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(nameLabelPtr.release());

    wing::Point textBoxLoc(16, 24 + 24 + 24);
    wing::Size textBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(60), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> textBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(textBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    textBox = textBoxPtr.get();
    textBox->TextChanged().AddHandler(this, &RuleDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(textBoxPtr.release()).Location(textBoxLoc).
        SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTextBoxPtr.release()).Location(textBoxLoc).
        SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedTextBoxPtr.release());

    wing::Point cascadeCheckBoxLoc(16, 16 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::CheckBox> cascadeCheckBoxkPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Cascade").Location(cascadeCheckBoxLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    cascadeCheckBox = cascadeCheckBoxkPtr.get();
    AddChild(cascadeCheckBoxkPtr.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Cancel").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    cancelButton = cancelButtonPtr.get();
    cancelButton->SetDialogResult(wing::DialogResult::cancel);
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;
    std::unique_ptr<wing::Button> okButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("OK").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    okButton = okButtonPtr.get();
    okButton->Disable();
    okButton->SetDialogResult(wing::DialogResult::ok);
    okButton->SetDefault();
    AddChild(okButtonPtr.release());
    AddChild(cancelButtonPtr.release());
    SetDefaultButton(okButton);
    SetCancelButton(cancelButton);

    textBox->SetFocus();
}

void RuleDialog::CheckValid()
{
    RuleKind ruleKind = static_cast<RuleKind>(ruleKindComboBox->GetSelectedIndex());
    if (ruleKind == RuleKind::exclude)
    {
        cascadeCheckBox->Enable();
    }
    else
    {
        cascadeCheckBox->SetChecked(false);
        cascadeCheckBox->Disable();
    }
    PathKind pathKind = static_cast<PathKind>(pathKindComboBox->GetSelectedIndex());
    if (!textBox->Text().empty())
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

void RuleDialog::SetData(Rule* rule)
{
    ruleKindComboBox->SetSelectedIndex(static_cast<int>(rule->GetRuleKind()));
    pathKindComboBox->SetSelectedIndex(static_cast<int>(rule->GetPathKind()));
    textBox->SetText(rule->Value());
    cascadeCheckBox->SetChecked(rule->Cascade());
}

void RuleDialog::GetData(Rule* rule)
{
    RuleKind ruleKind = static_cast<RuleKind>(ruleKindComboBox->GetSelectedIndex());
    rule->SetRuleKind(ruleKind);
    PathKind pathKind = static_cast<PathKind>(pathKindComboBox->GetSelectedIndex());
    rule->SetPathKind(pathKind);
    std::string name;
    if (ruleKind == RuleKind::exclude)
    {
        name.append("Exclude");
    }
    else if (ruleKind == RuleKind::include)
    {
        name.append("Include");
    }
    if (pathKind == PathKind::dir)
    {
        name.append(" Directory");
    }
    else if (pathKind == PathKind::file)
    {
        name.append(" File");
    }
    rule->SetName(name);
    rule->SetValue(textBox->Text());
    rule->SetCascade(cascadeCheckBox->Checked());
    if (ruleKind != RuleKind::include || pathKind != PathKind::dir)
    {
        rule->RemoveRules();
    }
}

} // package_editor

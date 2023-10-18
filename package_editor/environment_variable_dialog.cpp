// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.environment_variable_dialog;

import util;

namespace package_editor {

bool IsValidEnvironmentVariableName(const std::string& name)
{
    std::u32string str(util::ToUtf32(name));
    if (str.empty()) return false;
    char32_t first = str.front();
    if (!util::IsIdStart(first)) return false;
    int n = str.length();
    for (int i = 1; i < n; ++i)
    {
        if (!util::IsIdCont(str[i])) return false;
    }
    return true;
}

EnvironmentVariableDialog::EnvironmentVariableDialog(const std::string& caption) :
    wing::Window(wing::WindowCreateParams().WindowClassName("package_editor.environment_variable_dialog").Text(caption).
        WindowStyle(wing::DialogWindowStyle()).WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), wing::ScreenMetrics::Get().MMToVerticalPixels(80)))),
    okButton(nullptr),
    cancelButton(nullptr),
    nameTextBox(nullptr),
    valueTextBox(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point nameLabelLoc(16, 16);
    std::unique_ptr<wing::Label> nameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Name:").Location(nameLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(nameLabelPtr.release());

    wing::Point nameTextBoxLoc(16, 16 + 24);
    wing::Size textBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(60), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> nameTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(nameTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    nameTextBox = nameTextBoxPtr.get();
    nameTextBox->TextChanged().AddHandler(this, &EnvironmentVariableDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(nameTextBoxPtr.release()).Location(nameTextBoxLoc).
        SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTextBoxPtr.release()).Location(nameTextBoxLoc).
        SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedTextBoxPtr.release());

    wing::Point valueLabelLoc(16, 16 + 16 + 24 + 24);
    std::unique_ptr<wing::Label> valueLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Value:").Location(valueLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(valueLabelPtr.release());

    wing::Point valueTextBoxLoc(16, 16 + 16 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> valueTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(valueTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    valueTextBox = valueTextBoxPtr.get();
    std::unique_ptr<wing::PaddedControl> paddedValueTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(valueTextBoxPtr.release()).
        Location(valueTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedValueTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedValueTextBoxPtr.release()).
        Location(valueTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedValueTextBoxPtr.release());

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

    nameTextBox->SetFocus();
}

void EnvironmentVariableDialog::CheckValid()
{
    if (IsValidEnvironmentVariableName(nameTextBox->Text()))
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

void EnvironmentVariableDialog::SetData(EnvironmentVariable* environmentVariable)
{
    nameTextBox->SetText(environmentVariable->Name());
    valueTextBox->SetText(environmentVariable->Value());
}

void EnvironmentVariableDialog::GetData(EnvironmentVariable* environmentVariable)
{
    environmentVariable->SetName(nameTextBox->Text());
    environmentVariable->SetValue(valueTextBox->Text());
}

} // package_editor

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.component_dialog;

import util;

namespace package_editor {

bool IsValidComponentName(const std::string& name)
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

ComponentDialog::ComponentDialog(const std::string& caption_) :
    wing::Window(wing::WindowCreateParams().Text(caption_).WindowClassName("package_editor.component_dialog").
        WindowStyle(wing::DialogWindowStyle()).WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(100), wing::ScreenMetrics::Get().MMToVerticalPixels(60)))),
    okButton(nullptr),
    cancelButton(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point nameLabelLoc(16, 16);
    std::unique_ptr<wing::Label> nameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Name:").Location(nameLabelLoc).SetAnchors(
        static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(nameLabelPtr.release());

    wing::Point textBoxLoc(16, 16 + 24);
    wing::Size textBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(60), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> textBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(textBoxLoc).SetSize(textBoxSize).SetAnchors(
        static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    textBox = textBoxPtr.get();
    std::unique_ptr<wing::PaddedControl> paddedTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(textBoxPtr.release()).Location(textBoxLoc).
        SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTextBoxPtr.release()).Location(textBoxLoc).
        SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedTextBoxPtr.release());

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
    textBox->TextChanged().AddHandler(this, &ComponentDialog::TextBoxTextChanged);
}

void ComponentDialog::SetComponentName(const std::string& componentName)
{
    textBox->SetText(componentName);
    textBox->SelectAll();
}

void ComponentDialog::TextBoxTextChanged()
{
    std::string text = textBox->Text();
    if (IsValidComponentName(text))
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

} // package_editor

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.path_directory_dialog;

import package_editor.properties_view;

namespace package_editor {

PathDirectoryDialog::PathDirectoryDialog(const std::string& caption) :
    wing::Window(wing::WindowCreateParams().WindowClassName("package_editor.path_directory_dialog").Text(caption).
        WindowStyle(wing::DialogWindowStyle()).WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), wing::ScreenMetrics::Get().MMToVerticalPixels(80)))),
    okButton(nullptr),
    cancelButton(nullptr),
    pathTextBox(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point pathLabelLoc(16, 16);
    std::unique_ptr<wing::Label> pathLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Path:").Location(pathLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(pathLabelPtr.release());

    wing::Point pathTextBoxLoc(16, 16 + 24);
    wing::Size textBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(60), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> pathTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(pathTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    pathTextBox = pathTextBoxPtr.get();
    pathTextBox->TextChanged().AddHandler(this, &PathDirectoryDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(pathTextBoxPtr.release()).Location(pathTextBoxLoc).
        SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTextBoxPtr.release()).Location(pathTextBoxLoc).
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

    pathTextBox->SetFocus();
}

void PathDirectoryDialog::SetData(PathDirectory* pathDirectory)
{
    pathTextBox->SetText(pathDirectory->Value());
}

void PathDirectoryDialog::GetData(PathDirectory* pathDirectory)
{
    pathDirectory->SetName("PATH Directory");
    pathDirectory->SetValue(pathTextBox->Text());
}

void PathDirectoryDialog::CheckValid()
{
    if (IsValidPath(pathTextBox->Text()))
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

} // package_editor

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.shortcut_dialog;

import package_editor.properties_view;

namespace package_editor {

bool IsValidIconIndex(const std::string& text)
{
    if (text.empty()) return false;
    try
    {
        int iconIndex = std::stoi(text);
        if (iconIndex < 0)
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

ShortcutDialog::ShortcutDialog(const std::string& caption) :
    wing::Window(wing::WindowCreateParams().WindowClassName("package_editor.shortcut_dialog").Text(caption).
        WindowStyle(wing::DialogWindowStyle()).WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(200), wing::ScreenMetrics::Get().MMToVerticalPixels(150)))),
    okButton(nullptr),
    cancelButton(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point linkFilePathLabelLoc(16, 16);
    std::unique_ptr<wing::Label> linkFilePathLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Link File Path:").Location(linkFilePathLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(linkFilePathLabelPtr.release());

    wing::Point linkFilePathTextBoxLoc(16, 16 + 24);
    wing::Size textBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(100), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> linkFilePathTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(linkFilePathTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    linkFilePathTextBox = linkFilePathTextBoxPtr.get();
    linkFilePathTextBox->TextChanged().AddHandler(this, &ShortcutDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedLinkFilePathTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(linkFilePathTextBoxPtr.release()).
        Location(linkFilePathTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedLinkFilePathTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedLinkFilePathTextBoxPtr.release()).
        Location(linkFilePathTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedLinkFilePathTextBoxPtr.release());

    wing::Point pathLabelLoc(16, 16 + 16 + 24 + 24);
    std::unique_ptr<wing::Label> pathLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Path:").Location(pathLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(pathLabelPtr.release());

    wing::Point pathTextBoxLoc(16, 16 + 16 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> pathTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(pathTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    pathTextBox = pathTextBoxPtr.get();
    pathTextBox->TextChanged().AddHandler(this, &ShortcutDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedPathTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(pathTextBoxPtr.release()).Location(pathTextBoxLoc).
        SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedPathTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedPathTextBoxPtr.release()).
        Location(pathTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedPathTextBoxPtr.release());

    wing::Point argumentsLabelLoc(16, 16 + 16 + 16 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> argumentsLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Arguments:").Location(argumentsLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(argumentsLabelPtr.release());

    wing::Point argumentsTextBoxLoc(16, 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> argumentsTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(argumentsTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    argumentsTextBox = argumentsTextBoxPtr.get();
    argumentsTextBox->TextChanged().AddHandler(this, &ShortcutDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedArgumentsTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(argumentsTextBoxPtr.release()).
        Location(argumentsTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedArgumentsTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedArgumentsTextBoxPtr.release()).
        Location(argumentsTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedArgumentsTextBoxPtr.release());

    wing::Point workingDirectoryLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> workingDirectoryLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Working Directory:").Location(workingDirectoryLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(workingDirectoryLabelPtr.release());

    wing::Point workingDirectoryTextBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> workingDirectoryTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(workingDirectoryTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    workingDirectoryTextBox = workingDirectoryTextBoxPtr.get();
    workingDirectoryTextBox->TextChanged().AddHandler(this, &ShortcutDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedWorkingDirectoryTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(workingDirectoryTextBoxPtr.release()).
        Location(workingDirectoryTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedWorkingDirectoryTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedWorkingDirectoryTextBoxPtr.release()).
        Location(workingDirectoryTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedWorkingDirectoryTextBoxPtr.release());

    wing::Point descriptionLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> descriptionLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Description:").Location(descriptionLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(descriptionLabelPtr.release());

    wing::Point descriptionTextBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> descriptionTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(descriptionTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    descriptionTextBox = descriptionTextBoxPtr.get();
    descriptionTextBox->TextChanged().AddHandler(this, &ShortcutDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedDescriptionTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(descriptionTextBoxPtr.release()).
        Location(descriptionTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedDescriptionTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedDescriptionTextBoxPtr.release()).
        Location(descriptionTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedDescriptionTextBoxPtr.release());

    wing::Point iconFilePathLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> iconFilePathLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Icon File Path:").Location(iconFilePathLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(iconFilePathLabelPtr.release());

    wing::Point iconFilePathTextBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> iconFilePathTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(iconFilePathTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    iconFilePathTextBox = iconFilePathTextBoxPtr.get();
    iconFilePathTextBox->TextChanged().AddHandler(this, &ShortcutDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedIconFilePathTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(iconFilePathTextBoxPtr.release()).
        Location(iconFilePathTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedIconFilePathTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedIconFilePathTextBoxPtr.release()).
        Location(iconFilePathTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedIconFilePathTextBoxPtr.release());

    wing::Point iconIndexLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> iconIndexLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Icon Index:").Location(iconIndexLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(iconIndexLabelPtr.release());

    wing::Point iconIndexTextBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> iconIndexTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Text("0").Location(iconIndexTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    iconIndexTextBox = iconIndexTextBoxPtr.get();
    iconIndexTextBox->TextChanged().AddHandler(this, &ShortcutDialog::CheckValid);
    std::unique_ptr<wing::PaddedControl> paddedIconIndexTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(iconIndexTextBoxPtr.release()).
        Location(iconIndexTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedIconIndexTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedIconIndexTextBoxPtr.release()).
        Location(iconIndexTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedIconIndexTextBoxPtr.release());

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

    linkFilePathTextBox->SetFocus();
}

void ShortcutDialog::CheckValid()
{
    if (IsValidPath(linkFilePathTextBox->Text()) && IsValidPath(pathTextBox->Text()) && IsValidIconIndex(iconIndexTextBox->Text()))
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

void ShortcutDialog::GetData(Shortcut* shortcut)
{
    shortcut->SetName("Shortcut");
    shortcut->SetLinkFilePath(linkFilePathTextBox->Text());
    shortcut->SetPath(pathTextBox->Text());
    shortcut->SetArguments(argumentsTextBox->Text());
    shortcut->SetWorkingDirectory(workingDirectoryTextBox->Text());
    shortcut->SetDescription(descriptionTextBox->Text());
    shortcut->SetIconFilePath(iconFilePathTextBox->Text());
    shortcut->SetIconIndex(std::stoi(iconIndexTextBox->Text()));
}

void ShortcutDialog::SetData(Shortcut* shortcut)
{
    linkFilePathTextBox->SetText(shortcut->LinkFilePath());
    pathTextBox->SetText(shortcut->Path());
    argumentsTextBox->SetText(shortcut->Arguments());
    workingDirectoryTextBox->SetText(shortcut->WorkingDirectory());
    descriptionTextBox->SetText(shortcut->Description());
    iconFilePathTextBox->SetText(shortcut->IconFilePath());
    iconIndexTextBox->SetText(std::to_string(shortcut->IconIndex()));
}

} // package_editor

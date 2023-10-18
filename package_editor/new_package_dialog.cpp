// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.new_package_dialog;

import package_editor.main_window;
import package_editor.properties_view;
import util;

namespace package_editor {

NewPackageDialog::NewPackageDialog() :
    wing::Window(wing::WindowCreateParams().Text("New Package").WindowClassName("package_editor.new_package_dialog").
        WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(100))).WindowStyle(wing::DialogWindowStyle())),
    locationTextBox(nullptr), packageNameTextBox(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultComboBoxSize = wing::ScreenMetrics::Get().DefaultComboBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point packageLocationLabelLoc(16, 16);
    std::unique_ptr<wing::Label> packageLocationLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Package Location:").Location(packageLocationLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(packageLocationLabelPtr.release());

    wing::Point packageLocationTextBoxLoc(16, 16 + 24);
    wing::Size textBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(80), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> packageLocationTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Text(WingstallPackagesDir()).
        Location(packageLocationTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    locationTextBox = packageLocationTextBoxPtr.get();
    locationTextBox->TextChanged().AddHandler(this, &NewPackageDialog::CheckInput);
    std::unique_ptr<wing::PaddedControl> paddedPackageLocationTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(packageLocationTextBoxPtr.release()).
        Location(packageLocationTextBoxLoc).
        SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedPackageLocationTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedPackageLocationTextBoxPtr.release()).
        Location(packageLocationTextBoxLoc).
        SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedPackageLocationTextBoxPtr.release());

    wing::Point selectPackageLocationButtonLoc(16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(80), 16 + 24);
    std::unique_ptr<wing::Button> selectPackageLocationButton(new wing::Button(wing::ControlCreateParams().Text("...").Location(selectPackageLocationButtonLoc).
        SetSize(wing::Size(24, 24)).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    selectPackageLocationButton->Click().AddHandler(this, &NewPackageDialog::SelectPackageLocation);
    AddChild(selectPackageLocationButton.release());

    wing::Point packageNameLabelLoc(16, 16 + 16 + 24 + 24);
    std::unique_ptr<wing::Label> packageNameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Package Name:").Location(packageNameLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(packageNameLabelPtr.release());

    wing::Point packageNameTextBoxLoc(16, 16 + 16 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> packageNameTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(packageNameTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    packageNameTextBox = packageNameTextBoxPtr.get();
    packageNameTextBox->TextChanged().AddHandler(this, &NewPackageDialog::CheckInput);
    std::unique_ptr<wing::PaddedControl> paddedPackageNameTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(packageNameTextBoxPtr.release()).
        Location(packageNameTextBoxLoc).
        SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedPackageNameTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedPackageNameTextBoxPtr.release()).
        Location(packageNameTextBoxLoc).
        SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedPackageNameTextBoxPtr.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;

    wing::Point cancelButtonLoc(x, y);
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Location(cancelButtonLoc).SetSize(defaultButtonSize).
        Text("Cancel").SetAnchors(static_cast<wing::Anchors>(wing::Anchors::bottom | wing::Anchors::right))));
    wing::Button* cancelButton = cancelButtonPtr.get();
    cancelButton->SetDialogResult(wing::DialogResult::cancel);
    AddChild(cancelButtonPtr.release());

    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;

    wing::Point okButtonLoc(x, y);
    std::unique_ptr<wing::Button> okButtonPtr(new wing::Button(wing::ControlCreateParams().Location(okButtonLoc).SetSize(defaultButtonSize).Text("OK").
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::bottom | wing::Anchors::right))));
    okButton = okButtonPtr.get();
    okButton->Disable();
    okButton->SetDialogResult(wing::DialogResult::ok);
    okButton->SetDefault();
    SetDefaultButton(okButton);
    SetCancelButton(cancelButton);
    AddChild(okButtonPtr.release());

    locationTextBox->SetFocus();
}

void NewPackageDialog::CheckInput()
{
    if (IsValidPath(GetPackageFilePath()))
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

void NewPackageDialog::SelectPackageLocation()
{
    std::string packageLocation = util::Path::MakeCanonical(wing::SelectDirectory(nullptr, WingstallPackagesDir()));
    if (!packageLocation.empty())
    {
        locationTextBox->SetText(util::Path::MakeCanonical(packageLocation));
    }
}

std::string NewPackageDialog::GetPackageFilePath() const
{
    if (locationTextBox->Text().empty()) return std::string();
    if (packageNameTextBox->Text().empty()) return std::string();
    return util::GetFullPath(util::Path::Combine(util::Path::Combine(locationTextBox->Text(), packageNameTextBox->Text()), packageNameTextBox->Text() + ".package.xml"));
}

std::string NewPackageDialog::GetPackageName() const
{
    return packageNameTextBox->Text();
}

} // package_editor

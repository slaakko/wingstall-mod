// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module package_editor.edit_configuration_dialog;

import util;

namespace package_editor {

EditConfigurationDialog::EditConfigurationDialog(const std::string& boostIncludeDir, const std::string& boostLibDir, const std::string& vcVarsPath,
    const std::string& vcPlatformToolset) :
    wing::Window(wing::WindowCreateParams().Text("Edit Configuration").WindowClassName("package_editor.edit_configuration_dialog").
        WindowStyle(wing::DialogWindowStyle()).WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(180), wing::ScreenMetrics::Get().MMToVerticalPixels(100)))),
    okButton(nullptr),
    cancelButton(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point boostIncludedDirLabelLoc(16, 16);
    std::unique_ptr<wing::Label> boostIncludedDirLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Boost Include Directory:").Location(
        boostIncludedDirLabelLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(boostIncludedDirLabelPtr.release());

    wing::Point boostIncludedDirTextBoxLoc(16, 16 + 24);
    wing::Size textBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(140), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> boostIncludedDirTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(boostIncludedDirTextBoxLoc).Text(boostIncludeDir).
        SetSize(textBoxSize).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    boostIncludeDirTextBox = boostIncludedDirTextBoxPtr.get();
    boostIncludeDirTextBox->TextChanged().AddHandler(this, &EditConfigurationDialog::CheckInput);
    std::unique_ptr<wing::PaddedControl> paddedBoostIncludedDirTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(boostIncludedDirTextBoxPtr.release()).
        Location(boostIncludedDirTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedBoostIncludedDirTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedBoostIncludedDirTextBoxPtr.release()).
        Location(boostIncludedDirTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedBoostIncludedDirTextBoxPtr.release());

    wing::Point selectBoostIncludeDirButtonLoc(16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(140), 16 + 24);
    std::unique_ptr<wing::Button> selectBoostIncludeDirButton(new wing::Button(wing::ControlCreateParams().Text("...").Location(selectBoostIncludeDirButtonLoc).
        SetSize(wing::Size(24, 24)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    selectBoostIncludeDirButton->Click().AddHandler(this, &EditConfigurationDialog::SelectBoostIncludeDir);
    AddChild(selectBoostIncludeDirButton.release());

    wing::Point boostLibDirLabelLoc(16, 16 + 16 + 24 + 24);
    std::unique_ptr<wing::Label> boostLibDirLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Boost Library Directory:").Location(boostLibDirLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(boostLibDirLabelPtr.release());

    wing::Point boostLibDirTextBoxLoc(16, 16 + 16 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> boostLibDirTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(boostLibDirTextBoxLoc).Text(boostLibDir).
        SetSize(textBoxSize).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    boostLibDirTextBox = boostLibDirTextBoxPtr.get();
    boostLibDirTextBox->TextChanged().AddHandler(this, &EditConfigurationDialog::CheckInput);
    std::unique_ptr<wing::PaddedControl> paddedBoostLibDirTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(boostLibDirTextBoxPtr.release()).
        Location(boostLibDirTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedBoostLibDirTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedBoostLibDirTextBoxPtr.release()).
        Location(boostLibDirTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedBoostLibDirTextBoxPtr.release());

    wing::Point selectBoostLibDirButtonLoc(16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(140), 16 + 16 + 24 + 24 + 24);
    std::unique_ptr<wing::Button> selectBoostLibDirButton(new wing::Button(wing::ControlCreateParams().Text("...").Location(selectBoostLibDirButtonLoc).SetSize(wing::Size(24, 24)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    selectBoostLibDirButton->Click().AddHandler(this, &EditConfigurationDialog::SelectBoostLibDir);
    AddChild(selectBoostLibDirButton.release());

    wing::Point vcVarsPathLabelLoc(16, 16 + 16 + 16 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> vcVarsPathLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Visual C++ vcvars64.bat Path:").Location(vcVarsPathLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(vcVarsPathLabelPtr.release());

    wing::Point vcVarsPathTextBoxLoc(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> vcVarsPathTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Text(vcVarsPath).Location(vcVarsPathTextBoxLoc).SetSize(textBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    vcVarsPathTextBox = vcVarsPathTextBoxPtr.get();
    vcVarsPathTextBox->TextChanged().AddHandler(this, &EditConfigurationDialog::CheckInput);
    std::unique_ptr<wing::PaddedControl> paddedVCVarsPathTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(vcVarsPathTextBoxPtr.release()).
        Location(vcVarsPathTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedVCVarsPathTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedVCVarsPathTextBoxPtr.release()).
        Location(vcVarsPathTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedVCVarsPathTextBoxPtr.release());

    wing::Point selectVCVarsPathButtonLoc(16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(140), 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Button> selectVCVarsPathButton(new wing::Button(wing::ControlCreateParams().Text("...").Location(selectVCVarsPathButtonLoc).SetSize(wing::Size(24, 24)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    selectVCVarsPathButton->Click().AddHandler(this, &EditConfigurationDialog::SelectVCVarsPath);
    AddChild(selectVCVarsPathButton.release());

    wing::Point vcPlatformToolsetLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> vcPlatformToolsetLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Visual C++ Platform Toolset:").
        Location(vcPlatformToolsetLabelLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(vcPlatformToolsetLabelPtr.release());

    wing::Point vcPlatformToolsetTextBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> vcPlatformToolsetTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Text(vcPlatformToolset).
        Location(vcPlatformToolsetTextBoxLoc).SetSize(textBoxSize).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    vcPlatformToolsetTextBox = vcPlatformToolsetTextBoxPtr.get();
    vcPlatformToolsetTextBox->TextChanged().AddHandler(this, &EditConfigurationDialog::CheckInput);
    std::unique_ptr<wing::PaddedControl> paddedVCPlatformToolsetTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(vcPlatformToolsetTextBoxPtr.release()).
        Location(vcPlatformToolsetTextBoxLoc).SetSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding())).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedVCPlatformToolsetTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(
        paddedVCPlatformToolsetTextBoxPtr.release()).Location(vcPlatformToolsetTextBoxLoc).
        SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedVCPlatformToolsetTextBoxPtr.release());

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

    boostIncludeDirTextBox->SetFocus();

    CheckInput();
}

void EditConfigurationDialog::GetData(std::string& boostIncludeDir, std::string& boostLibDir, std::string& vcVarsPath, std::string& vcPlatformToolset)
{
    boostIncludeDir = util::MakeNativePath(boostIncludeDirTextBox->Text());
    boostLibDir = util::MakeNativePath(boostLibDirTextBox->Text());
    vcVarsPath = util::MakeNativePath(vcVarsPathTextBox->Text());
    vcPlatformToolset = vcPlatformToolsetTextBox->Text();
}

void EditConfigurationDialog::CheckInput()
{
    if (!boostIncludeDirTextBox->Text().empty() && !boostLibDirTextBox->Text().empty() && !vcVarsPathTextBox->Text().empty() && !vcPlatformToolsetTextBox->Text().empty())
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

void EditConfigurationDialog::SelectBoostIncludeDir()
{
    std::string boostIncludeDir = util::Path::MakeCanonical(wing::SelectDirectory(nullptr, boostIncludeDirTextBox->Text()));
    if (!boostIncludeDir.empty())
    {
        boostIncludeDirTextBox->SetText(util::Path::MakeCanonical(boostIncludeDir));
    }
}

void EditConfigurationDialog::SelectBoostLibDir()
{
    std::string boostLibDir = util::Path::MakeCanonical(wing::SelectDirectory(nullptr, boostLibDirTextBox->Text()));
    if (!boostLibDir.empty())
    {
        boostLibDirTextBox->SetText(util::Path::MakeCanonical(boostLibDir));
    }
}

void EditConfigurationDialog::SelectVCVarsPath()
{
    std::vector<std::pair<std::string, std::string>> descriptionFilterPairs;
    descriptionFilterPairs.push_back(std::make_pair("Batch Files (*.bat)", " *.bat"));
    std::string initialDirectory = util::Path::GetDirectoryName(util::Path::MakeCanonical(vcVarsPathTextBox->Text()));
    std::string filePath;
    std::string currentDirectory;
    std::vector<std::string> fileNames;
    bool selected = wing::OpenFileName(Handle(), descriptionFilterPairs, initialDirectory, std::string(), "bat", 
        OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filePath, currentDirectory, fileNames);
    if (selected)
    {
        std::string vcVarsPath = util::GetFullPath(filePath);
        vcVarsPathTextBox->SetText(vcVarsPath);
    }
}

} // package_editor

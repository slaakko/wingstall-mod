// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef MessageBox

module package_editor.properties_view;

import package_editor.package;
import package_editor.main_window;
import util;
import std.filesystem;

namespace package_editor {

bool IsValidPath(const std::string& text)
{
    if (text.empty()) return false;
    std::u32string str = util::ToUtf32(text);
    for (char32_t c : str)
    {
        switch (c)
        {
        case '<': case '>': case '"': case '|': case '?': case '*': return false;
        }
    }
    return true;
}

bool IsValidProductId(const std::string& text)
{
    if (text.empty()) return false;
    try
    {
        util::uuid productId = util::ParseUuid(text);
    }
    catch (const std::exception&)
    {
        return false;
    }
    return true;
}

PropertiesView::PropertiesView(Package* package_) :
    wing::ContainerControl(wing::ControlCreateParams().BackgroundColor(wing::DefaultControlBackgroundColor()).WindowClassName("package_editor.properties_view").SetDock(wing::Dock::fill)),
    package(package_), mainWindow(package->GetMainWindow()), exitHandlerId(-1), compressionComboBox(nullptr), dirty(false), initializing(true)
{
    bool enableApplyButton = false;
    ResetCaretDisabled();
    SetDoubleBuffered();
    wing::Size s = GetSize();
    defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultComboBoxSize = wing::ScreenMetrics::Get().DefaultComboBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point sourceRootDirLabelLoc(16, 16);
    std::unique_ptr<wing::Label> sourceRootDirLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Source Root Directory:").Location(sourceRootDirLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(sourceRootDirLabelPtr.release());

    wing::Point sourceRootDirTextBoxLoc(16, 16 + 24);

    wing::Size textBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(80), defaultTextBoxSize.Height + 4);
    wing::Size sourceRootDirTextBoxSize(textBoxSize);
    std::unique_ptr<wing::TextBox> sourceRootDirTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(sourceRootDirTextBoxLoc).
        Text(package->GetProperties()->SourceRootDir()).SetSize(sourceRootDirTextBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    if (!std::filesystem::exists(package->GetProperties()->SourceRootDir()))
    {
        enableApplyButton = true;
    }
    sourceRootDirTextBox = sourceRootDirTextBoxPtr.get();
    sourceRootDirTextBox->TextChanged().AddHandler(this, &PropertiesView::EnableApply);
    std::unique_ptr<wing::PaddedControl> paddedSourceRootDirTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(sourceRootDirTextBoxPtr.release()).
        SetSize(wing::PaddedSize(sourceRootDirTextBoxSize, wing::DefaultPadding())).
        Location(sourceRootDirTextBoxLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedSourceRootDirTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedSourceRootDirTextBoxPtr.release()).
        Text(package->GetProperties()->SourceRootDir()).Location(sourceRootDirTextBoxLoc).SetSize(
            wing::BorderedSize(wing::PaddedSize(sourceRootDirTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedSourceRootDirTextBoxPtr.release());

    wing::Point selectSourceRootDirButtonLoc(16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(80), 16 + 24);
    std::unique_ptr<wing::Button> selectSourceRootDirButton(new wing::Button(wing::ControlCreateParams().Text("...").Location(selectSourceRootDirButtonLoc).SetSize(wing::Size(24, 24)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    selectSourceRootDirButton->Click().AddHandler(this, &PropertiesView::SelectSourceRootDir);
    AddChild(selectSourceRootDirButton.release());

    wing::Point sourceRootDirRelativeCheckBoxLoc(16 + 16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(80) + 24, 16 + 24 + 4);
    std::unique_ptr<wing::CheckBox> sourceRootDirRelativeCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::left | wing::Anchors::top)).Text("Relative Path").Location(sourceRootDirRelativeCheckBoxLoc)));
    sourceRootDirRelativeCheckBox = sourceRootDirRelativeCheckBoxPtr.get();
    sourceRootDirRelativeCheckBox->CheckedChanged().AddHandler(this, &PropertiesView::SourceRootDirRelativeCheckBoxCheckedChanged);
    if (util::Path::IsRelative(package->GetProperties()->SourceRootDir()))
    {
        sourceRootDirRelativeCheckBox->SetChecked(true);
    }
    AddChild(sourceRootDirRelativeCheckBoxPtr.release());

    wing::Point targetRootDirLabelLoc(16, 16 + 16 + 24 + 24);
    std::unique_ptr<wing::Label> targetRootDirLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Target Root Directory:").Location(targetRootDirLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(targetRootDirLabelPtr.release());

    wing::Point targetRootDirTextBoxLoc(16, 16 + 16 + 24 + 24 + 24);
    wing::Size targetRootDirTextBoxSize(textBoxSize);
    if (package->GetProperties()->TargetRootDir().empty())
    {
        enableApplyButton = true;
    }
    std::unique_ptr<wing::TextBox> targetRootDirTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(targetRootDirTextBoxLoc).
        Text(package->GetProperties()->TargetRootDir()).SetSize(targetRootDirTextBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    targetRootDirTextBox = targetRootDirTextBoxPtr.get();
    targetRootDirTextBox->TextChanged().AddHandler(this, &PropertiesView::EnableApply);
    std::unique_ptr<wing::PaddedControl> paddedTargetRootDirTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(targetRootDirTextBoxPtr.release()).
        SetSize(wing::PaddedSize(targetRootDirTextBoxSize, wing::DefaultPadding())).
        Location(targetRootDirTextBoxLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedTargetRootDirTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTargetRootDirTextBoxPtr.release()).
        Location(targetRootDirTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(targetRootDirTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedTargetRootDirTextBoxPtr.release());

    wing::Point appNameLabelLoc(16, 16 + 16 + 16 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> appNameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Application Name:").Location(appNameLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(appNameLabelPtr.release());

    wing::Point appNameTextBoxLoc(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24);
    wing::Size appNameTextBoxSize(textBoxSize);
    std::unique_ptr<wing::TextBox> appNameTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(appNameTextBoxLoc).Text(package->GetProperties()->AppName()).
        SetSize(appNameTextBoxSize).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    appNameTextBox = appNameTextBoxPtr.get();
    appNameTextBox->TextChanged().AddHandler(this, &PropertiesView::EnableApply);
    std::unique_ptr<wing::PaddedControl> paddedAppNameTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(appNameTextBoxPtr.release()).
        SetSize(wing::PaddedSize(appNameTextBoxSize, wing::DefaultPadding())).
        Location(appNameTextBoxLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedAppNameTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedAppNameTextBoxPtr.release()).
        Location(appNameTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(appNameTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedAppNameTextBoxPtr.release());

    wing::Point publisherLabelLoc(16, 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> publisherLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Publisher:").Location(publisherLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(publisherLabelPtr.release());

    wing::Point publisherTextBoxLoc(16, 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size publisherTextBoxSize(textBoxSize);
    std::unique_ptr<wing::TextBox> publisherTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(publisherTextBoxLoc).Text(package->GetProperties()->Publisher()).
        SetSize(publisherTextBoxSize).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    publisherTextBox = publisherTextBoxPtr.get();
    publisherTextBox->TextChanged().AddHandler(this, &PropertiesView::EnableApply);
    std::unique_ptr<wing::PaddedControl> paddedPublisherTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(publisherTextBoxPtr.release()).
        SetSize(wing::PaddedSize(publisherTextBoxSize, wing::DefaultPadding())).
        Location(publisherTextBoxLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedPublisherTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedPublisherTextBoxPtr.release()).
        Location(publisherTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(publisherTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedPublisherTextBoxPtr.release());

    wing::Point versionLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> versionLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Version:").Location(versionLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(versionLabelPtr.release());

    wing::Point versionTextBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size versionTextBoxSize(textBoxSize);
    std::unique_ptr<wing::TextBox> versionTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(versionTextBoxLoc).Text(package->GetProperties()->Version()).
        SetSize(versionTextBoxSize).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    versionTextBox = versionTextBoxPtr.get();
    versionTextBox->TextChanged().AddHandler(this, &PropertiesView::EnableApply);
    std::unique_ptr<wing::PaddedControl> paddedVersionTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(versionTextBoxPtr.release()).
        SetSize(wing::PaddedSize(versionTextBoxSize, wing::DefaultPadding())).
        Location(versionTextBoxLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedVersionTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedVersionTextBoxPtr.release()).
        Location(versionTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(versionTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedVersionTextBoxPtr.release());

    wing::Point productIdLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> productIdLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Product ID:").Location(productIdLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(productIdLabelPtr.release());

    wing::Point productIdTextBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size productIdTextBoxSize(textBoxSize);
    std::unique_ptr<wing::TextBox> productIdTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(productIdTextBoxLoc).
        Text(util::ToString(package->GetProperties()->Id())).SetSize(productIdTextBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    productIdTextBox = productIdTextBoxPtr.get();
    productIdTextBox->TextChanged().AddHandler(this, &PropertiesView::EnableApply);
    std::unique_ptr<wing::PaddedControl> paddedProductIdTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(productIdTextBoxPtr.release()).
        SetSize(wing::PaddedSize(productIdTextBoxSize, wing::DefaultPadding())).
        Location(productIdTextBoxLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedProductIdTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedProductIdTextBoxPtr.release()).
        Location(productIdTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(productIdTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedProductIdTextBoxPtr.release());

    wing::Point createProduceIdButtonLoc(16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(80), 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Button> createProduceIdButton(new wing::Button(wing::ControlCreateParams().Text("Random ID").Location(createProduceIdButtonLoc).SetSize(defaultButtonSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    createProduceIdButton->Click().AddHandler(this, &PropertiesView::CreateProductId);
    AddChild(createProduceIdButton.release());

    wing::Point compressionLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> compressionLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Compression:").Location(compressionLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(compressionLabelPtr.release());

    wing::Point compressionComboBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size compressionComboBoxSize(defaultComboBoxSize.Width, defaultComboBoxSize.Height * 6);
    std::unique_ptr<wing::ComboBox> compressionComboBoxPtr(new wing::ComboBox(wing::ComboBoxCreateParams().Location(compressionComboBoxLoc).SetSize(compressionComboBoxSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    compressionComboBox = compressionComboBoxPtr.get();
    compressionComboBox->SelectedIndexChanged().AddHandler(this, &PropertiesView::EnableApply);
    AddChild(compressionComboBoxPtr.release());

    wing::Point iconFilePathLabelLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> iconFilePathLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Icon File Path:").Location(iconFilePathLabelLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(iconFilePathLabelPtr.release());

    wing::Point iconFilePathTextBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size iconFilePathTextBoxSize(textBoxSize);
    std::unique_ptr<wing::TextBox> iconFilePathTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Location(iconFilePathTextBoxLoc).
        Text(package->GetProperties()->IconFilePath()).SetSize(iconFilePathTextBoxSize).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    iconFilePathTextBox = iconFilePathTextBoxPtr.get();
    iconFilePathTextBox->TextChanged().AddHandler(this, &PropertiesView::EnableApply);
    std::unique_ptr<wing::PaddedControl> paddedIconFilePathTextBoxPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(iconFilePathTextBoxPtr.release()).
        SetSize(wing::PaddedSize(iconFilePathTextBoxSize, wing::DefaultPadding())).
        Location(iconFilePathTextBoxLoc).SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    std::unique_ptr<wing::BorderedControl> borderedIconFilePathTextBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedIconFilePathTextBoxPtr.release()).
        Location(iconFilePathTextBoxLoc).SetSize(wing::BorderedSize(wing::PaddedSize(iconFilePathTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    AddChild(borderedIconFilePathTextBoxPtr.release());

    wing::Point includeUninstallerCheckBoxLoc(16, 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::CheckBox> includeUninstallerCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Include uninstaller").Location(includeUninstallerCheckBoxLoc).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::top | wing::Anchors::left))));
    includeUninstallerCheckBox = includeUninstallerCheckBoxPtr.get();
    includeUninstallerCheckBox->SetChecked(package->GetProperties()->IncludeUninstaller());
    includeUninstallerCheckBox->CheckedChanged().AddHandler(this, &PropertiesView::EnableApply);
    AddChild(includeUninstallerCheckBoxPtr.release());

    wing::Point applyButtonLocation(wing::ScreenMetrics::Get().MMToHorizontalPixels(100), 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);

    std::unique_ptr<wing::Button> applyButtonPtr(new wing::Button(wing::ControlCreateParams().Location(applyButtonLocation).Text("Apply").SetSize(defaultButtonSize).
        SetAnchors(static_cast<wing::Anchors>(wing::Anchors::left | wing::Anchors::top))));
    applyButton = applyButtonPtr.get();
    applyButton->Click().AddHandler(this, &PropertiesView::ApplyButtonClick);
    AddChild(applyButtonPtr.release());

    SetScrollUnits(wing::ScrollUnits(17, 17));
    SetContentSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(100), applyButtonLocation.Y + defaultButtonSize.Height));

    MainWindow* mainWindow = package->GetMainWindow();
    if (mainWindow)
    {
        exitHandlerId = mainWindow->ExitView().AddHandler(this, &PropertiesView::Exit);
    }

    dirty = false;
    initializing = false;
    applyButton->Disable();
    if (enableApplyButton)
    {
        dirty = true;
        applyButton->Enable();
    }

    if (mainWindow)
    {
        wing::KeyPreviewMethod keypreview;
        keypreview.SetHandlerFunction(this, &PropertiesView::KeyPreview);
        mainWindow->SetKeyPreviewMethod(keypreview);
        wing::Application::SetActiveWindow(mainWindow);
    }
}

PropertiesView::~PropertiesView()
{
    wing::Application::SetActiveWindow(nullptr);
    if (mainWindow)
    {
        if (exitHandlerId != -1)
        {
            mainWindow->ExitView().RemoveHandler(exitHandlerId);
            exitHandlerId = -1;
        }
    }
}

void PropertiesView::KeyPreview(wing::Keys keys, bool& handled)
{
    if (keys == wing::Keys::tab)
    {
        mainWindow->FocusNext();
        handled = true;
    }
    else if (keys == (wing::Keys::shiftModifier | wing::Keys::tab))
    {
        mainWindow->FocusPrev();
        handled = true;
    }
}

void PropertiesView::OnCreated()
{
    ContainerControl::OnCreated();
    compressionComboBox->AddItem("none");
    compressionComboBox->AddItem("deflate");
    int compression = static_cast<int>(package->GetProperties()->GetCompression());
    compressionComboBox->SetSelectedIndex(compression);
}

void PropertiesView::OnSizeChanged()
{
    wing::Size sz = GetSize();
    if (sz.Width > 0)
    {
        applyButton->SetLocation(wing::Point(sz.Width - defaultButtonSize.Width - 4 * defaultControlSpacing.Width, applyButton->Location().Y));
    }
}

void PropertiesView::EnableApply()
{
    dirty = true;
    applyButton->Enable();
}

bool PropertiesView::CheckApplyChanges()
{
    if (!IsValidPath(sourceRootDirTextBox->Text()))
    {
        sourceRootDirTextBox->SelectAll();
        wing::ShowErrorMessageBox(Handle(), "Source root directory path not valid");
        return false;
    }
    if (!IsValidPath(targetRootDirTextBox->Text()))
    {
        targetRootDirTextBox->SelectAll();
        wing::ShowErrorMessageBox(Handle(), "Target root directory path not valid");
        return false;
    }
    if (appNameTextBox->Text().empty())
    {
        appNameTextBox->SelectAll();
        wing::ShowErrorMessageBox(Handle(), "Application name cannot be empty");
        return false;
    }
    if (!IsValidProductId(productIdTextBox->Text()))
    {
        productIdTextBox->SelectAll();
        wing::ShowErrorMessageBox(Handle(), "Invalid product ID");
        return false;
    }
    return true;
}

void PropertiesView::ApplyButtonClick()
{
    if (CheckApplyChanges())
    {
        if (!sourceRootDirTextBox->Text().empty())
        {
            if (util::Path::IsAbsolute(sourceRootDirTextBox->Text()))
            {
                std::filesystem::create_directories(util::MakeNativeFileSystemPath(sourceRootDirTextBox->Text()));
            }
        }
        package->GetProperties()->SetSourceRootDir(sourceRootDirTextBox->Text());
        package->GetProperties()->SetTargetRootDir(targetRootDirTextBox->Text());
        package->GetProperties()->SetAppName(appNameTextBox->Text());
        package->GetProperties()->SetPublisher(publisherTextBox->Text());
        package->GetProperties()->SetVersion(versionTextBox->Text());
        package->GetProperties()->SetCompression(static_cast<wing_package::Compression>(compressionComboBox->GetSelectedIndex()));
        package->GetProperties()->SetIconFilePath(iconFilePathTextBox->Text());
        package->GetProperties()->SetIncludeUninstaller(includeUninstallerCheckBox->Checked());
        package->GetProperties()->SetId(util::ParseUuid(productIdTextBox->Text()));
        package->GetComponents()->RemoveUnexistingDirectoriesAndFiles();
        package->GetEngineVariables()->Fetch();
        applyButton->Disable();
        dirty = false;
        package->SetDirty();
    }
    else
    {
        applyButton->Enable();
    }
}

void PropertiesView::SelectSourceRootDir()
{
    MainWindow* mainWindow = package->GetMainWindow();
    if (mainWindow)
    {
        std::string sourceRootDir = util::Path::MakeCanonical(wing::SelectDirectory(nullptr, "C:/"));
        if (!sourceRootDir.empty())
        {
            initializing = true;
            if (util::Path::IsRelative(sourceRootDir))
            {
                sourceRootDirRelativeCheckBox->SetChecked(true);
            }
            else
            {
                sourceRootDirRelativeCheckBox->SetChecked(false);
            }
            sourceRootDirTextBox->SetText(util::Path::MakeCanonical(sourceRootDir));
            initializing = false;
        }
    }
}

void PropertiesView::CreateProductId()
{
    util::uuid newProductId = util::random_uuid();
    productIdTextBox->SetText(util::ToString(newProductId));
}

void PropertiesView::SourceRootDirRelativeCheckBoxCheckedChanged()
{
    if (initializing) return;
    std::string referenceDirPath = util::Path::GetDirectoryName(package->FilePath());
    if (sourceRootDirRelativeCheckBox->Checked())
    {
        std::string absoluteDirPath = sourceRootDirTextBox->Text();
        if (util::Path::IsAbsolute(absoluteDirPath))
        {
            std::string relativeDirPath = util::MakeRelativeDirPath(absoluteDirPath, referenceDirPath);
            sourceRootDirTextBox->SetText(relativeDirPath);
        }
    }
    else
    {
        std::string relativeDirPath = sourceRootDirTextBox->Text();
        if (util::Path::IsRelative(relativeDirPath))
        {
            std::string absoluteDirPath = util::GetFullPath(util::Path::Combine(referenceDirPath, relativeDirPath));
            sourceRootDirTextBox->SetText(absoluteDirPath);
        }
    }
}

void PropertiesView::Exit(wing::CancelArgs& args)
{
    wing::Application::SetActiveWindow(nullptr);
    mainWindow->SetFocusedControl(nullptr);
    if (dirty)
    {
        wing::MessageBoxResult result = wing::MessageBox::Show("Apply changes?", "Package | Properties", this, MB_YESNOCANCEL);
        if (result == wing::MessageBoxResult::cancel)
        {
            args.cancel = true;
        }
        else
        {
            if (result == wing::MessageBoxResult::yes)
            {
                if (!CheckApplyChanges())
                {
                    args.cancel = true;
                    return;
                }
                applyButton->DoClick();
            }
            else if (result == wing::MessageBoxResult::no)
            {
                dirty = false;
                applyButton->Disable();
            }
            MainWindow* mainWindow = package->GetMainWindow();
            if (mainWindow)
            {
                if (exitHandlerId != -1)
                {
                    mainWindow->ExitView().RemoveHandler(exitHandlerId);
                    exitHandlerId = -1;
                }
            }
        }
    }
}

} // package_editor

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing_gui.install_window;

import wing_gui.action_dialog;
import wing_gui.message_dialog;
import util;
import std.filesystem;
//#include <codecvt>

namespace wing_gui {

std::string GetAppName()
{
    wing_package::InfoItem* appNameItem = GetInfoItem(wing_package::InfoItemKind::appName);
    if (appNameItem && appNameItem->Type() == wing_package::InfoItemType::string)
    {
        return static_cast<wing_package::StringItem*>(appNameItem)->Value();
    }
    return std::string();
}

std::string GetAppVersion()
{
    wing_package::InfoItem* appVersionItem = GetInfoItem(wing_package::InfoItemKind::appVersion);
    if (appVersionItem && appVersionItem->Type() == wing_package::InfoItemType::string)
    {
        return static_cast<wing_package::StringItem*>(appVersionItem)->Value();
    }
    return std::string();
}

std::string GetInstallDirName()
{
    wing_package::InfoItem* installDirNameItem = GetInfoItem(wing_package::InfoItemKind::installDirName);
    if (installDirNameItem && installDirNameItem->Type() == wing_package::InfoItemType::string)
    {
        return static_cast<wing_package::StringItem*>(installDirNameItem)->Value();
    }
    return std::string();
}

std::string GetDefaultContainingDirPath()
{
    wing_package::InfoItem* defaultContainingDirPathItem = GetInfoItem(wing_package::InfoItemKind::defaultContainingDirPath);
    if (defaultContainingDirPathItem && defaultContainingDirPathItem->Type() == wing_package::InfoItemType::string)
    {
        return static_cast<wing_package::StringItem*>(defaultContainingDirPathItem)->Value();
    }
    return std::string();
}

std::string GetDefaultInstallationDirPath()
{
    return util::Path::Combine(GetDefaultContainingDirPath(), GetInstallDirName());
}

std::string GetInstallWindowCaption()
{
    std::string installWindowCaption;
    installWindowCaption.append(GetAppName()).append(" ").append(GetAppVersion()).append(" installation");
    return installWindowCaption;
}

int64_t GetUncompressedPackageSize()
{
    wing_package::InfoItem* uncompressedPackageSizeItem = GetInfoItem(wing_package::InfoItemKind::uncompressedPackageSize);
    if (uncompressedPackageSizeItem && uncompressedPackageSizeItem->Type() == wing_package::InfoItemType::integer)
    {
        return static_cast<wing_package::IntegerItem*>(uncompressedPackageSizeItem)->Value();
    }
    return 0;
}

int64_t GetCompressedPackageSize()
{
    wing_package::InfoItem* compressedPackageSizeItem = GetInfoItem(wing_package::InfoItemKind::compressedPackageSize);
    if (compressedPackageSizeItem && compressedPackageSizeItem->Type() == wing_package::InfoItemType::integer)
    {
        return static_cast<wing_package::IntegerItem*>(compressedPackageSizeItem)->Value();
    }
    return 0;
}

std::string GetPackageFilePath()
{
    wing_package::InfoItem* packageFilePathItem = GetInfoItem(wing_package::InfoItemKind::packageFilePath);
    if (packageFilePathItem && packageFilePathItem->Type() == wing_package::InfoItemType::string)
    {
        return static_cast<wing_package::StringItem*>(packageFilePathItem)->Value();
    }
    return std::string();
}

int64_t GetPackageAddress()
{
    wing_package::InfoItem* packageAddressItem = GetInfoItem(wing_package::InfoItemKind::packageDataAddress);
    if (packageAddressItem && packageAddressItem->Type() == wing_package::InfoItemType::integer)
    {
        return static_cast<wing_package::IntegerItem*>(packageAddressItem)->Value();
    }
    return 0;
}

wing_package::Compression GetCompression()
{
    wing_package::InfoItem* compressionItem = GetInfoItem(wing_package::InfoItemKind::compression);
    if (compressionItem && compressionItem->Type() == wing_package::InfoItemType::integer)
    {
        return static_cast<wing_package::Compression>(static_cast<wing_package::IntegerItem*>(compressionItem)->Value());
    }
    return wing_package::Compression::none;
}

wing_package::DataSource GetDataSource()
{
    wing_package::InfoItem* dataSourceItem = GetInfoItem(wing_package::InfoItemKind::dataSource);
    if (dataSourceItem && dataSourceItem->Type() == wing_package::InfoItemType::integer)
    {
        return static_cast<wing_package::DataSource>(static_cast<wing_package::IntegerItem*>(dataSourceItem)->Value());
    }
    return wing_package::DataSource::file;
}

void RunInstallation(InstallWindow* installWindow, const std::string& installationDir)
{
    try
    {
        installWindow->RunInstallation(installationDir);
    }
    catch (...)
    {
    }
}

InstallWindowPackageObserver::InstallWindowPackageObserver(InstallWindow* installWindow_) : installWindow(installWindow_)
{
}

void InstallWindowPackageObserver::StatusChanged(wing_package::Package* package)
{
    installWindow->PutStatusMessage(new StatusChangedMessage(package->GetStatus(), package->GetStatusStr(), package->GetErrorMessage()));
}

void InstallWindowPackageObserver::ComponentChanged(wing_package::Package* package)
{
    wing_package::Node* component = package->GetComponent();
    if (component)
    {
        installWindow->PutStatusMessage(new ComponentChangedMessage(component->FullName()));
    }
    else
    {
        installWindow->PutStatusMessage(new ComponentChangedMessage(std::string()));
    }
}

void InstallWindowPackageObserver::FileChanged(wing_package::Package* package)
{
    wing_package::File* file = package->GetFile();
    if (file)
    {
        installWindow->PutStatusMessage(new FileChangedMessage(file->Name()));
    }
    else
    {
        installWindow->PutStatusMessage(new FileChangedMessage(std::string()));
    }
}

void InstallWindowPackageObserver::StreamPositionChanged(wing_package::Package* package)
{
    int64_t position = package->GetStreamPosition();
    int64_t size = GetUncompressedPackageSize();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    if (position >= size || now - installWindow->UpdateTime() >= std::chrono::milliseconds{ updateIntegervalMs })
    {
        installWindow->SetUpdateTime(now);
        installWindow->PutStatusMessage(new StreamPositionChangedMessage(position));
    }
}

void InstallWindowPackageObserver::LogError(wing_package::Package* packge, const std::string& error)
{
    installWindow->PutStatusMessage(new LogErrorMessage(error));
}

InstallWindow::InstallWindow() : wing::Window(wing::WindowCreateParams().WindowStyle(wing::DialogWindowStyle()).Text(GetInstallWindowCaption()).
    WindowClassName("wing_gui.InstallWindow").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(120)))),
    editMode(false), closeButton(nullptr), installButton(nullptr), containingDirLabel(nullptr), containingDirTextBox(nullptr),
    installationDirLabel(nullptr), installationDirTextBox(nullptr), statusLabel(nullptr), statusText(nullptr), containingDirButton(nullptr),
    componentLabel(nullptr), componentText(nullptr), fileLabel(nullptr), fileText(nullptr), progressLabel(nullptr), progressPercentText(nullptr), progressBar(nullptr),
    observer(this), package(nullptr), updateTime(), installationRunning(false)
{
    std::u16string ffName(util::ToUtf16("Segoe UI"));
    const WCHAR* fontFamilyName = (const WCHAR*)ffName.c_str();
    wing::FontFamily family(fontFamilyName);
    boldFont = wing::Font(family, wing::DefaultWindowFontSize(), wing::FontStyle::FontStyleBold, wing::Unit::UnitPoint);
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();
    wing::Size defaultCheckBoxSize = wing::ScreenMetrics::Get().DefaultCheckBoxSize();

    wing::Point containingDirLabelLocation(16, 16);
    std::unique_ptr<wing::Label> containingDirLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Containing directory:").Location(containingDirLabelLocation).
        SetSize(defaultLabelSize).SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    containingDirLabel = containingDirLabelPtr.get();
    containingDirLabel->SetFont(boldFont);
    AddChild(containingDirLabelPtr.release());

    wing::Point containingDirTextBoxLocation(16, 16 + defaultControlSpacing.Height + 16);
    wing::Size containingDirTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> containingDirTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    containingDirTextBox = containingDirTextBoxPtr.get();
    containingDirTextBox->TextChanged().AddHandler(this, &InstallWindow::ContainingDirTextBoxTextChanged);
    std::unique_ptr<wing::Control> paddedContainingDirTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(containingDirTextBoxPtr.release()).
        SetSize(wing::PaddedSize(containingDirTextBoxSize, wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedContainingDirTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedContainingDirTextBox.release()).
        Location(containingDirTextBoxLocation).SetSize(wing::BorderedSize(wing::PaddedSize(containingDirTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(borderedContainingDirTextBox.release());
    containingDirTextBox->SetText(util::MakeNativePath(GetDefaultContainingDirPath()));

    wing::Point containingDirButtonLocation(wing::ScreenMetrics::Get().MMToHorizontalPixels(120) + 32, 16 + defaultControlSpacing.Height + 16);
    wing::Size containingDirButtonSize(24, 24);
    std::unique_ptr<wing::Button> containingDirButtonPtr(new wing::Button(wing::ControlCreateParams().Location(containingDirButtonLocation).SetSize(containingDirButtonSize).
        Text("...").SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    containingDirButton = containingDirButtonPtr.get();
    containingDirButton->Click().AddHandler(this, &InstallWindow::SelectContainingDirButtonClick);
    AddChild(containingDirButtonPtr.release());

    wing::Point installationDirLabelLocation(16, 16 + 16 + 24 + 24);
    std::unique_ptr<wing::Label> installationDirLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Installation directory:").Location(installationDirLabelLocation).
        SetSize(defaultLabelSize).SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    installationDirLabel = installationDirLabelPtr.get();
    installationDirLabel->SetFont(boldFont);
    AddChild(installationDirLabelPtr.release());

    wing::Point installationDirTextBoxLocation(16, 16 + defaultControlSpacing.Height + 16 + 16 + 24 + 24);
    wing::Size installationDirTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> installationDirTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    installationDirTextBox = installationDirTextBoxPtr.get();
    std::unique_ptr<wing::Control> paddedInstallationDirTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(installationDirTextBoxPtr.release()).
        SetSize(wing::PaddedSize(installationDirTextBoxSize, wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedInstallationDirTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedInstallationDirTextBox.release()).
        Location(installationDirTextBoxLocation).SetSize(wing::BorderedSize(wing::PaddedSize(installationDirTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(borderedInstallationDirTextBox.release());
    installationDirTextBox->SetText(util::MakeNativePath(GetDefaultInstallationDirPath()));

    wing::Point statusLabelLocation(16, 16 + 16 + 16 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> statusLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Status:").Location(statusLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    statusLabel = statusLabelPtr.get();
    statusLabel->SetFont(boldFont);
    AddChild(statusLabelPtr.release());

    wing::Size statusTextSize = defaultLabelSize;
    statusTextSize.Width = s.Width - 24 - 24;
    wing::Point statusTextLocation(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> statusTextPtr(new wing::Label(wing::LabelCreateParams().Location(statusTextLocation).AutoSize(false).SetSize(statusTextSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    statusText = statusTextPtr.get();
    AddChild(statusTextPtr.release());

    wing::Point componentLabelLocation(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> componentLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Component:").Location(componentLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    componentLabel = componentLabelPtr.get();
    componentLabel->SetFont(boldFont);
    AddChild(componentLabelPtr.release());

    wing::Size componentTextSize = defaultLabelSize;
    componentTextSize.Width = s.Width - 24 - 24;
    wing::Point componentTextLocation(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> componentTextPtr(new wing::Label(wing::LabelCreateParams().Location(componentTextLocation).AutoSize(false).SetSize(componentTextSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    componentText = componentTextPtr.get();
    AddChild(componentTextPtr.release());

    wing::Point fileLabelLocation(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> fileLabelPtr(new wing::Label(wing::LabelCreateParams().Text("File:").Location(fileLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    fileLabel = fileLabelPtr.get();
    fileLabel->SetFont(boldFont);
    AddChild(fileLabelPtr.release());

    wing::Size fileTextSize = defaultLabelSize;
    fileTextSize.Width = s.Width - 24 - 24;
    wing::Point fileTextLocation(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> fileTextPtr(new wing::Label(wing::LabelCreateParams().Location(fileTextLocation).AutoSize(false).SetSize(fileTextSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    fileText = fileTextPtr.get();
    AddChild(fileTextPtr.release());

    wing::Point progressLabelLocation(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> progressLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Progress:").Location(progressLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    progressLabel = progressLabelPtr.get();
    progressLabel->SetFont(boldFont);
    AddChild(progressLabelPtr.release());

    wing::Point progressBarLocation(16, 16 + 16 + 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size progressBarSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height);
    std::unique_ptr<wing::ProgressBar> progressBarPtr(new wing::ProgressBar(wing::ProgressBarCreateParams().Defaults().Location(progressBarLocation).SetSize(
        wing::PaddedSize(progressBarSize, wing::DefaultPadding()))));
    progressBar = progressBarPtr.get();
    AddChild(progressBarPtr.release());

    wing::Point progressPercentTextLocation(16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(120) + wing::defaultControlPadding, 16 + 16 + 16 + 3 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> progressPercentTextPtr(new wing::Label(wing::LabelCreateParams().Text("0%").Location(progressPercentTextLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    progressPercentText = progressPercentTextPtr.get();
    AddChild(progressPercentTextPtr.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> closeButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Close").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    closeButton = closeButtonPtr.get();
    closeButton->SetDialogResult(wing::DialogResult::cancel);
    closeButton->Click().AddHandler(this, &InstallWindow::CloseButtonClick);
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;
    std::unique_ptr<wing::Button> installButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Install").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    installButton = installButtonPtr.get();
    installButton->Click().AddHandler(this, &InstallWindow::InstallButtonClick);
    installButton->SetDialogResult(wing::DialogResult::ok);
    installButton->SetDefault();
    AddChild(installButtonPtr.release());
    AddChild(closeButtonPtr.release());
    SetDefaultButton(installButton);
    SetCancelButton(closeButton);
    containingDirTextBox->SetFocus();
    editMode = true;
}

void InstallWindow::PutStatusMessage(StatusMessage* statusMessage)
{
    statusMessageQueue.Put(statusMessage);
    SendMessage(Handle(), STATUS_MESSAGE_AVAILABLE, 0, 0);
}

bool InstallWindow::ProcessMessage(wing::Message& msg)
{
    switch (msg.message)
    {
    case STATUS_MESSAGE_AVAILABLE:
    {
        HandleStatusMessage();
        msg.result = 0;
        return true;
    }
    default:
    {
        return wing::Window::ProcessMessage(msg);
    }
    }
}

void InstallWindow::HandleStatusMessage()
{
    std::unique_ptr<StatusMessage> message = statusMessageQueue.Get();
    switch (message->Kind())
    {
    case StatusMessageKind::statusChanged:
    {
        StatusChangedMessage* statusChanged = static_cast<StatusChangedMessage*>(message.get());
        SetStatus(statusChanged->GetStatus(), statusChanged->StatusStr(), statusChanged->ErrorMessage());
        break;
    }
    case StatusMessageKind::componentChanged:
    {
        ComponentChangedMessage* componentChanged = static_cast<ComponentChangedMessage*>(message.get());
        SetComponent(componentChanged->Component());
        break;
    }
    case StatusMessageKind::fileChanged:
    {
        FileChangedMessage* fileChanged = static_cast<FileChangedMessage*>(message.get());
        SetFile(fileChanged->File());
        break;
    }
    case StatusMessageKind::streamPositionChanged:
    {
        StreamPositionChangedMessage* streamPositionChanged = static_cast<StreamPositionChangedMessage*>(message.get());
        int64_t position = streamPositionChanged->Position();
        SetStreamPosition(position);
        break;
    }
    case StatusMessageKind::logError:
    {
        LogErrorMessage* errorMessage = static_cast<LogErrorMessage*>(message.get());
        MessageDialog dialog(MessageKind::error, "error: " + errorMessage->Error());
        dialog.ShowDialog(*this);
        break;
    }
    }
}

void InstallWindow::SetPackage(wing_package::Package* package_)
{
    package = package_;
    package->AddObserver(&observer);
}

InstallWindow::~InstallWindow()
{
    try
    {
        if (package)
        {
            package->RemoveObserver(&observer);
        }
        if (thread.joinable())
        {
            thread.join();
        }
    }
    catch (...)
    {
    }
}

void InstallWindow::SetProgressPercent(int64_t pos, int64_t size)
{
    int percent = 0;
    if (size > 0)
    {
        if (pos == size)
        {
            percent = 100;
        }
        else if (pos < size)
        {
            percent = (100.0 * pos) / size;
        }
        else
        {
            percent = 100;
        }
    }
    progressBar->SetProgressPercent(percent);
    progressPercentText->SetText(std::to_string(percent) + "%");
}

void InstallWindow::SetStreamPosition(int64_t position)
{
    int64_t packageSize = GetUncompressedPackageSize();
    SetProgressPercent(position, packageSize);
}

std::string InstallWindow::GetInstallationDir() const
{
    std::string installationDir = installationDirTextBox->Text();
    return installationDir;
}

void InstallWindow::SetStatus(wing_package::Status status, const std::string& statusStr, const std::string& errorMessage)
{
    statusText->SetText(statusStr);
    if (status == wing_package::Status::failed)
    {
        MessageDialog dialog(MessageKind::error, statusStr + ": " + errorMessage);
        dialog.ShowDialog(*this);
    }
    else if (status == wing_package::Status::aborted)
    {
        MessageDialog dialog(MessageKind::info, statusStr);
        dialog.ShowDialog(*this);
    }
}

void InstallWindow::SetComponent(const std::string& component)
{
    componentText->SetText(component);
}

void InstallWindow::SetFile(const std::string& file)
{
    fileText->SetText(util::MakeNativePath(file));
}

void InstallWindow::RunInstallation(const std::string& installationDir)
{
    try
    {
        if (package)
        {
            if (!installationDir.empty())
            {
                package->SetTargetRootDir(util::GetFullPath(installationDir));
                wing_package::DataSource dataSource = GetDataSource();
                if (dataSource == wing_package::DataSource::file)
                {
                    std::string packageFilePath = GetPackageFilePath();
                    if (!packageFilePath.empty())
                    {
                        package->Install(wing_package::DataSource::file, packageFilePath, nullptr, 0, wing_package::Content::all);
                    }
                }
                else if (dataSource == wing_package::DataSource::memory)
                {
                    int64_t address = GetPackageAddress();
                    if (address != 0)
                    {
                        uint8_t* data = static_cast<uint8_t*>(reinterpret_cast<void*>(address));
                        int64_t size = GetCompressedPackageSize();
                        package->Install(wing_package::DataSource::memory, std::string(), data, size, wing_package::Content::all);
                    }
                }
                installationRunning = false;
            }
            else
            {
                installationRunning = false;
            }
        }
        else
        {
            installationRunning = false;
        }
    }
    catch (const std::exception& ex)
    {
        installationRunning = false;
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void InstallWindow::InstallButtonClick()
{
    try
    {
        installationRunning = true;
        installButton->Disable();
        containingDirTextBox->Disable();
        containingDirButton->Disable();
        installationDirTextBox->Disable();
        thread = std::thread{ wing_gui::RunInstallation, this, GetInstallationDir() };
    }
    catch (const std::exception& ex)
    {
        installationRunning = false;
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void InstallWindow::CloseButtonClick()
{
    try
    {
        if (installationRunning)
        {
            if (package)
            {
                package->Interrupt();
                ActionDialog actionDialog(ActionDialogKind::installationAction);
                wing::DialogResult result = actionDialog.ShowDialog(*this);
                if (result == wing::DialogResult::abort)
                {
                    package->SetAction(wing_package::Action::abortAction);
                }
                else if (result == wing::DialogResult::ok)
                {
                    package->SetAction(wing_package::Action::continueAction);
                }
            }
            else
            {
                Close();
            }
        }
        else
        {
            Close();
        }
    }
    catch (const std::exception& ex)
    {
        installationRunning = false;
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void InstallWindow::ContainingDirTextBoxTextChanged()
{
    if (editMode)
    {
        installationDirTextBox->SetText(util::MakeNativePath(util::Path::Combine(util::Path::MakeCanonical(containingDirTextBox->Text()), GetInstallDirName())));
    }
}

void InstallWindow::InstallationDirTextBoxTextChanged()
{
    if (editMode)
    {
        if (!installationDirTextBox->Text().empty())
        {
            installButton->Enable();
        }
        else
        {
            installButton->Disable();
        }
    }
}

void InstallWindow::SelectContainingDirButtonClick()
{
    std::string location = wing::SelectDirectory(nullptr, util::Path::MakeCanonical(containingDirTextBox->Text()));
    if (!location.empty())
    {
        containingDirTextBox->SetText(location);
    }
}

} // namespace wing_gui



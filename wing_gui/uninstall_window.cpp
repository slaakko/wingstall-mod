// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing_gui.uninstall_window;

import wing_gui.action_dialog;
import util;

namespace wing_gui {

UninstallWindowPackageObserver::UninstallWindowPackageObserver(UninstallWindow* uninstallWindow_) : uninstallWindow(uninstallWindow_)
{
}

void UninstallWindowPackageObserver::StatusChanged(wing_package::Package* package)
{
    uninstallWindow->PutStatusMessage(new StatusChangedMessage(package->GetStatus(), package->GetStatusStr(), package->GetErrorMessage()));
}

void UninstallWindowPackageObserver::ComponentChanged(wing_package::Package* package)
{
    wing_package::Node* component = package->GetComponent();
    if (component)
    {
        uninstallWindow->PutStatusMessage(new ComponentChangedMessage(component->FullName()));
    }
    else
    {
        uninstallWindow->PutStatusMessage(new ComponentChangedMessage(std::string()));
    }
}

void UninstallWindowPackageObserver::FileChanged(wing_package::Package* package)
{
    wing_package::File* file = package->GetFile();
    if (file)
    {
        uninstallWindow->PutStatusMessage(new FileChangedMessage(file->Name()));
    }
    else
    {
        uninstallWindow->PutStatusMessage(new FileChangedMessage(std::string()));
    }
}

void UninstallWindowPackageObserver::FileIndexChanged(wing_package::Package* package)
{
    uninstallWindow->PutStatusMessage(new FileIndexChangedMessage(package->FileIndex()));
}

void UninstallWindowPackageObserver::LogError(wing_package::Package* package, const std::string& error)
{
    uninstallWindow->PutStatusMessage(new LogErrorMessage(error));
}

void RunUninstallation(UninstallWindow* uninstallWindow)
{
    try
    {
        uninstallWindow->RunUninstallation();
    }
    catch (...)
    {
    }
}

std::string GetUninstallWindowCaption()
{
    std::string uninstallWindowCaption;
    uninstallWindowCaption.append(GetAppName()).append(" ").append(GetAppVersion()).append(" uninstallation");
    return uninstallWindowCaption;
}

UninstallWindow::UninstallWindow() : wing::Window(wing::WindowCreateParams().WindowStyle(wing::DialogWindowStyle()).Text(GetUninstallWindowCaption()).
    WindowClassName("wing_gui.UninstallWindow").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(120)))),
    package(nullptr), boldFont(), closeButton(), uninstallButton(), progressBar(), thread(), uninstallationRunning(false), observer(this),
    statusLabel(nullptr), statusText(nullptr), componentLabel(nullptr), componentText(nullptr), fileLabel(nullptr), fileText(nullptr),
    progressLabel(nullptr), progressPercentText(nullptr)
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

    wing::Point statusLabelLocation(16, 16);
    std::unique_ptr<wing::Label> statusLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Status:").Location(statusLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    statusLabel = statusLabelPtr.get();
    statusLabel->SetFont(boldFont);
    AddChild(statusLabelPtr.release());

    wing::Size statusTextSize = defaultLabelSize;
    statusTextSize.Width = s.Width - 24 - 24;
    wing::Point statusTextLocation(16, 16 + 24);
    std::unique_ptr<wing::Label> statusTextPtr(new wing::Label(wing::LabelCreateParams().Location(statusTextLocation).AutoSize(false).SetSize(statusTextSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    statusText = statusTextPtr.get();
    AddChild(statusTextPtr.release());

    wing::Point componentLabelLocation(16, 16 + 24 + 24);
    std::unique_ptr<wing::Label> componentLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Component:").Location(componentLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    componentLabel = componentLabelPtr.get();
    componentLabel->SetFont(boldFont);
    AddChild(componentLabelPtr.release());

    wing::Size componentTextSize = defaultLabelSize;
    componentTextSize.Width = s.Width - 24 - 24;
    wing::Point componentTextLocation(16, 16 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> componentTextPtr(new wing::Label(wing::LabelCreateParams().Location(componentTextLocation).AutoSize(false).SetSize(componentTextSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    componentText = componentTextPtr.get();
    AddChild(componentTextPtr.release());

    wing::Point fileLabelLocation(16, 16 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> fileLabelPtr(new wing::Label(wing::LabelCreateParams().Text("File:").Location(fileLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    fileLabel = fileLabelPtr.get();
    fileLabel->SetFont(boldFont);
    AddChild(fileLabelPtr.release());

    wing::Size fileTextSize = defaultLabelSize;
    fileTextSize.Width = s.Width - 24 - 24;
    wing::Point fileTextLocation(16, 16 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> fileTextPtr(new wing::Label(wing::LabelCreateParams().Location(fileTextLocation).AutoSize(false).SetSize(fileTextSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    fileText = fileTextPtr.get();
    AddChild(fileTextPtr.release());

    wing::Point progressLabelLocation(16, 16 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> progressLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Progress:").Location(progressLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    progressLabel = progressLabelPtr.get();
    progressLabel->SetFont(boldFont);
    AddChild(progressLabelPtr.release());

    wing::Point progressBarLocation(16, 16 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size progressBarSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height);
    std::unique_ptr<wing::ProgressBar> progressBarPtr(new wing::ProgressBar(wing::ProgressBarCreateParams().Defaults().Location(progressBarLocation).SetSize(
        wing::PaddedSize(progressBarSize, wing::DefaultPadding()))));
    progressBar = progressBarPtr.get();
    AddChild(progressBarPtr.release());

    wing::Point progressPercentTextLocation(16 + 16 + wing::ScreenMetrics::Get().MMToHorizontalPixels(120) + wing::defaultControlPadding, 16 + 24 + 24 + 3 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> progressPercentTextPtr(new wing::Label(wing::LabelCreateParams().Text("0%").Location(progressPercentTextLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    progressPercentText = progressPercentTextPtr.get();
    AddChild(progressPercentTextPtr.release());

    wing::Point logLabelLocation(16, 16 + 24 + 24 + 8 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> logLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Log:").Location(logLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    logLabel = logLabelPtr.get();
    logLabel->SetFont(boldFont);
    AddChild(logLabelPtr.release());

    wing::Point logLocation(16, 16 + 24 + 24 + 8 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size logSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), wing::ScreenMetrics::Get().MMToHorizontalPixels(30));
    std::unique_ptr<wing::LogView> logPtr(new wing::LogView(wing::TextViewCreateParams().Location(logLocation).SetSize(logSize)));
    log = logPtr.get();
    log->SetFlag(wing::ControlFlags::scrollSubject);
    std::unique_ptr<wing::Control> scrollableLog(new wing::ScrollableControl(wing::ScrollableControlCreateParams(logPtr.release()).Defaults()));
    std::unique_ptr<wing::Control> paddedLog(new wing::PaddedControl(wing::PaddedControlCreateParams(scrollableLog.release()).Location(logLocation).SetSize(
        wing::PaddedSize(logSize, wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedLog(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedLog.release()).SetBorderStyle(wing::BorderStyle::single).
        Location(logLocation).SetSize(wing::PaddedSize(wing::BorderedSize(logSize, wing::BorderStyle::single), wing::DefaultPadding()))));
    AddChild(borderedLog.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> closeButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Close").SetAnchors(
        wing::Anchors::right | wing::Anchors::bottom)));
    closeButton = closeButtonPtr.get();
    closeButton->SetDialogResult(wing::DialogResult::cancel);
    closeButton->Click().AddHandler(this, &UninstallWindow::CloseButtonClick);
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;
    std::unique_ptr<wing::Button> uninstallButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Uninstall").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    uninstallButton = uninstallButtonPtr.get();
    uninstallButton->Click().AddHandler(this, &UninstallWindow::UninstallButtonClick);
    uninstallButton->SetDialogResult(wing::DialogResult::ok);
    uninstallButton->SetDefault();
    AddChild(uninstallButtonPtr.release());
    AddChild(closeButtonPtr.release());
    SetDefaultButton(uninstallButton);
    SetCancelButton(closeButton);
}

UninstallWindow::~UninstallWindow()
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

void UninstallWindow::SetPackage(wing_package::Package* package_)
{
    package = package_;
    package->AddObserver(&observer);
}

bool UninstallWindow::ProcessMessage(wing::Message& msg)
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
        return Window::ProcessMessage(msg);
    }
    }
}

void UninstallWindow::HandleStatusMessage()
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
    case StatusMessageKind::fileIndexChanged:
    {
        FileIndexChangedMessage* fileIndexChanged = static_cast<FileIndexChangedMessage*>(message.get());
        SetFileIndex(fileIndexChanged->FileIndex());
        break;
    }
    case StatusMessageKind::logError:
    {
        LogErrorMessage* errorMessage = static_cast<LogErrorMessage*>(message.get());
        log->WriteLine("error: " + errorMessage->Error());
        break;
    }
    }
}

void UninstallWindow::SetStatus(wing_package::Status status, const std::string& statusStr, const std::string& errorMessage)
{
    statusText->SetText(statusStr);
    if (status == wing_package::Status::succeeded)
    {
        log->WriteLine(statusStr);
    }
    else if (status == wing_package::Status::running)
    {
        log->WriteLine(statusStr);
    }
    else if (status == wing_package::Status::failed)
    {
        log->WriteLine(statusStr + ": " + errorMessage);
    }
    else if (status == wing_package::Status::aborted)
    {
        log->WriteLine(statusStr);
    }
}

void UninstallWindow::SetComponent(const std::string& component)
{
    componentText->SetText(component);
}

void UninstallWindow::SetFile(const std::string& file)
{
    fileText->SetText(util::MakeNativePath(file));
}

void UninstallWindow::SetFileIndex(int fileIndex)
{
    int fileCount = package->FileCount();
    SetProgressPercent(fileIndex, fileCount);
}

void UninstallWindow::SetProgressPercent(int fileIndex, int fileCount)
{
    int percent = 0;
    if (fileCount > 0)
    {
        if (fileIndex == fileCount)
        {
            percent = 100;
        }
        else if (fileIndex < fileCount)
        {
            percent = (100.0 * fileIndex) / fileCount;
        }
        else
        {
            percent = 100;
        }
    }
    progressBar->SetProgressPercent(percent);
    progressPercentText->SetText(std::to_string(percent) + "%");
}

void UninstallWindow::UninstallButtonClick()
{
    try
    {
        uninstallationRunning = true;
        uninstallButton->Disable();
        thread = std::thread{ wing_gui::RunUninstallation, this };
    }
    catch (const std::exception& ex)
    {
        uninstallationRunning = false;
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void UninstallWindow::CloseButtonClick()
{
    try
    {
        if (uninstallationRunning)
        {
            if (package)
            {
                package->Interrupt();
                ActionDialog actionDialog(ActionDialogKind::uninstallationAction);
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
        uninstallationRunning = false;
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void UninstallWindow::RunUninstallation()
{
    try
    {
        if (package)
        {
            package->Uninstall();
            uninstallationRunning = false;
        }
        else
        {
            uninstallationRunning = false;
        }
    }
    catch (const std::exception& ex)
    {
        uninstallationRunning = false;
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void UninstallWindow::PutStatusMessage(StatusMessage* statusMessage)
{
    statusMessageQueue.Put(statusMessage);
    SendMessage(Handle(), STATUS_MESSAGE_AVAILABLE, 0, 0);
}

} // namespace wing_gui

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_gui.install_window;

import wing_gui.status_message;
import wing_package;
import wing;
import std.core;

export namespace wing_gui {

const int64_t updateIntegervalMs = 250;

class InstallWindow;

void RunInstallation(InstallWindow* installWindow, const std::string& installationDir);
std::string GetAppName();
std::string GetAppVersion();


class InstallWindowPackageObserver : public wing_package::PackageObserver
{
public:
    InstallWindowPackageObserver(InstallWindow* installWindow_);
    void StatusChanged(wing_package::Package* package) override;
    void ComponentChanged(wing_package::Package* package) override;
    void FileChanged(wing_package::Package* package) override;
    void StreamPositionChanged(wing_package::Package* package) override;
    void LogError(wing_package::Package* package, const std::string& error) override;
private:
    InstallWindow* installWindow;
};

class InstallWindow : public wing::Window
{
public:
    InstallWindow();
    ~InstallWindow();
    void SetPackage(wing_package::Package* package_);
    void SetProgressPercent(int64_t pos, int64_t size);
    void SetStatus(wing_package::Status status, const std::string& statusStr, const std::string& errorMessage);
    void SetComponent(const std::string& component);
    void SetFile(const std::string& filePath);
    void SetStreamPosition(int64_t position);
    std::string GetInstallationDir() const;
    void PutStatusMessage(StatusMessage* statusMessage);
    void HandleStatusMessage();
    void RunInstallation(const std::string& installationDir);
    std::chrono::steady_clock::time_point UpdateTime() const { return updateTime; }
    void SetUpdateTime(std::chrono::steady_clock::time_point updateTime_) { updateTime = updateTime_; }
protected:
    bool ProcessMessage(wing::Message& msg) override;
private:
    void InstallButtonClick();
    void CloseButtonClick();
    void ContainingDirTextBoxTextChanged();
    void InstallationDirTextBoxTextChanged();
    void SelectContainingDirButtonClick();
    wing_package::Package* package;
    bool editMode;
    wing::Font boldFont;
    wing::Button* closeButton;
    wing::Button* installButton;
    wing::Button* containingDirButton;
    wing::Label* containingDirLabel;
    wing::TextBox* containingDirTextBox;
    wing::Label* installationDirLabel;
    wing::TextBox* installationDirTextBox;
    wing::Label* statusLabel;
    wing::Label* statusText;
    wing::Label* componentLabel;
    wing::Label* componentText;
    wing::Label* fileLabel;
    wing::Label* fileText;
    wing::Label* progressLabel;
    wing::Label* progressPercentText;
    wing::ProgressBar* progressBar;
    InstallWindowPackageObserver observer;
    StatusMessageQueue statusMessageQueue;
    std::thread thread;
    std::chrono::steady_clock::time_point updateTime;
    bool installationRunning;
};

} // namespace wing_gui

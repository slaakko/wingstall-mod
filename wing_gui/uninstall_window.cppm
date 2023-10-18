// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_gui.uninstall_window;

import wing_gui.status_message;
import wing_gui.install_window;
import wing_package;
import wing;
import std.core;

export namespace wing_gui {

class UninstallWindow;

class UninstallWindowPackageObserver : public wing_package::PackageObserver
{
public:
    UninstallWindowPackageObserver(UninstallWindow* uninstallWindow_);
    void StatusChanged(wing_package::Package* package) override;
    void ComponentChanged(wing_package::Package* package) override;
    void FileChanged(wing_package::Package* package) override;
    void FileIndexChanged(wing_package::Package* package) override;
    void LogError(wing_package::Package* package, const std::string& error) override;
private:
    UninstallWindow* uninstallWindow;
};

class UninstallWindow : public wing::Window
{
public:
    UninstallWindow();
    ~UninstallWindow();
    void SetPackage(wing_package::Package* package_);
    void RunUninstallation();
    void PutStatusMessage(StatusMessage* statusMessage);
protected:
    bool ProcessMessage(wing::Message& msg) override;
private:
    void HandleStatusMessage();
    void SetStatus(wing_package::Status status, const std::string& statusStr, const std::string& errorMessage);
    void SetComponent(const std::string& component);
    void SetFile(const std::string& file);
    void SetFileIndex(int fileIndex);
    void SetProgressPercent(int fileIndex, int fileCount);
    void UninstallButtonClick();
    void CloseButtonClick();
    wing_package::Package* package;
    wing::Font boldFont;
    wing::Label* statusLabel;
    wing::Label* statusText;
    wing::Label* componentLabel;
    wing::Label* componentText;
    wing::Label* fileLabel;
    wing::Label* fileText;
    wing::Label* progressLabel;
    wing::Label* progressPercentText;
    wing::Label* logLabel;
    wing::LogView* log;
    wing::Button* closeButton;
    wing::Button* uninstallButton;
    wing::ProgressBar* progressBar;
    std::thread thread;
    bool uninstallationRunning;
    UninstallWindowPackageObserver observer;
    StatusMessageQueue statusMessageQueue;
};

} // namespace wing_gui

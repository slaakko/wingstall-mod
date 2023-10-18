// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.main_window;

import package_editor.package;
import package_editor.package_explorer;
import package_editor.package_content_view;
import package_editor.tool_bar;
import package_editor.build;
import package_editor.configuration;
import wing;
import std.core;

export namespace package_editor {

class PathBar;

using ExitViewEvent = wing::EventWithArgs<wing::CancelArgs>;

std::string WingstallPackagesDir();

class MainWindow : public wing::Window
{
public:
    MainWindow();
    ~MainWindow();
    PathBar* GetPathBar() const { return pathBar; }
    wing::ImageList* GetImageList() const { return const_cast<wing::ImageList*>(&imageList); }
    void AddListViewEventHandlers(wing::ListView* listView);
    void AddTreeViewEventHandlers(wing::TreeView* treeView);
    void ClearClickActions();
    std::vector<std::unique_ptr<wing::ClickAction>>& ClickActions() { return clickActions; }
    ExitViewEvent& ExitView() { return exitView; }
    void ShowPackageFilePathStatusItems();
    void HidePackageFilePathStatusItems();
    void EnableSave();
    void DisableSave();
    void ShowPackageBuildProgressStatusItems();
    void HidePackageBuildProgressStatusItems();
    void StartBuild();
    void CancelBuildClick();
    void EndBuild();
    void ClosePackageClick();
    Package* GetPackage() const { return package.get(); }
    wing::LogView* GetLog() const { return logView; }
    void SetBuildProgressPercent(int percent);
    void ListViewColumnWidthChanged(wing::ListViewColumnEventArgs& args);
protected:
    void OnTimer(wing::TimerEventArgs& args) override;
    void OnKeyDown(wing::KeyEventArgs& args) override;
    void MouseUpNotification(wing::MouseEventArgs& args) override;
    void OnWindowStateChanged() override;
    void OnSizeChanged() override;
    void OnWindowClosing(wing::CancelArgs& args) override;
private:
    void NewPackageClick();
    void OpenPackageClick();
    void SavePackageClick();
    void BuildPackageClick();
    void OpenBinDirectoryClick();
    void ExitClick();
    void EditConfigurationClick();
    void HomepageClick();
    void LocalDocumentationClick();
    void AboutClick();
    void ResetConfigClick();
    void SetCommandStatus();
    void PackageExplorerSizeChanged();
    void PackageContentViewSizeChanged();
    void PathComponentSelected(wing::PathComponentArgs& args);
    void ListViewItemClick(wing::ListViewItemEventArgs& args);
    void ListViewItemRightClick(wing::ListViewItemEventArgs& args);
    void ListViewItemDoubleClick(wing::ListViewItemEventArgs& args);
    void ParentPathSelectorClick();
    void LoadConfigurationSettings();
    void SetWindowSettings();
    wing::SplitContainer* horizontalSplitContainer;
    wing::SplitContainer* verticalSplitContainer;
    wing::ToolButton* saveToolButton;
    wing::ToolButton* buildToolButton;
    wing::ToolButton* cancelBuildToolButton;
    wing::ToolButton* openBinDirectoryToolButton;
    wing::MenuItem* newPackageMenuItem;
    wing::MenuItem* openPackageMenuItem;
    wing::MenuItem* savePackageMenuItem;
    wing::MenuItem* buildPackageMenuItem;
    wing::MenuItem* cancelBuildMenuItem;
    wing::MenuItem* closePackageMenuItem;
    wing::MenuItem* exitMenuItem;
    wing::MenuItem* editConfigurationMenuItem;
    wing::MenuItem* openBinDirectoryMenuItem;
    wing::MenuItem* homepageMenuItem;
    wing::MenuItem* localDocumentationMenuItem;
    wing::MenuItem* aboutMenuItem;
    wing::MenuItem* resetMenuItem;
    wing::StatusBarTextItem* packageFilePathLabelStatusBarItem;
    wing::StatusBarTextItem* packageFilePathStatusBarItem;
    wing::StatusBarTextItem* packageBuildProgressLabelStatusBarItem;
    wing::StatusBarControlItem* buildProgressStatusBarItem;
    wing::ProgressBar* buildProgressBar;
    wing::StatusBarTextItem* packageBuildProgressPerceStatusBarItem;
    ExitViewEvent exitView;
    std::unique_ptr<Package> package;
    PathBar* pathBar;
    PackageExplorer* packageExplorer;
    PackageContentView* packageContentView;
    wing::LogView* logView;
    wing::ImageList imageList;
    bool showingDialog;
    std::vector<std::unique_ptr<wing::ClickAction>> clickActions;
    bool canceled;
    bool setMaximizedSplitterDistance;
    std::unique_ptr<BuildTask> buildTask;
    std::unique_ptr<std::thread> buildThread;
};

} // package_editor

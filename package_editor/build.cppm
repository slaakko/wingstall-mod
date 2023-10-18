// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.build;

import wing;
import wing_package;
import std.core;

export namespace package_editor {

class MainWindow;

class BuildTask : wing_package::PackageObserver
{
public:
    BuildTask(MainWindow* mainWindow_);
    void Run();
    void Cancel();
    void StatusChanged(wing_package::Package* package) override;
    void LogError(wing_package::Package* package, const std::string& error) override;
    void FileContentPositionChanged(wing_package::Package* package) override;
    void FileCopyProgress(int64_t pos, int64_t size);
private:
    void MakeCompileBatches(
        const std::string& packageBinFilePath,
        std::string& compileDebugFilePath,
        std::string& debugStdoutFilePath,
        std::string& debugStderrFilePath,
        std::string& compileReleaseFilePath,
        std::string& releaseStdoutFilePath,
        std::string& releaseStderrFilePath);
    void Compile(const std::string& compileBatFilePath, const std::string& stdOutFilePath, const std::string& stdErrFilePath, int startPercent, int endPercent);
    void Copy(const std::string& setupFilePath, const std::string& binDirectory);
    bool buildCanceled;
    wing_package::Package* package;
    MainWindow* mainWindow;
    wing::LogView* log;
    int prevPercent;
};

void RunBuildTask(BuildTask* buildTask);

} // package_editor

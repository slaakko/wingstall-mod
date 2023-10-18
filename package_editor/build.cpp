// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.build;

import package_editor.main_window;
import wingstall_config;
import dom;
import util;
import std.filesystem;

namespace package_editor {

class BuildLogger : public wing_package::Logger
{
public:
    BuildLogger(wing::LogView* log_);
    void WriteLine(const std::string& line) override;
private:
    wing::LogView* log;
};

BuildLogger::BuildLogger(wing::LogView* log_) : log(log_)
{
}

void BuildLogger::WriteLine(const std::string& line)
{
    log->WriteLine(line);
}

class BuildCopyFileObserver : public util::CopyFileObserver
{
public:
    BuildCopyFileObserver(wing::LogView* log_, BuildTask* task_);
    void WriteLine(const std::string& line) override;
    void FileCopyProgress(int64_t pos, int64_t size) override;
private:
    wing::LogView* log;
    BuildTask* task;
};

BuildCopyFileObserver::BuildCopyFileObserver(wing::LogView* log_, BuildTask* task_) : log(log_), task(task_)
{
}

void BuildCopyFileObserver::WriteLine(const std::string& line)
{
    if (log)
    {
        log->WriteLine(line);
    }
}

void BuildCopyFileObserver::FileCopyProgress(int64_t pos, int64_t size)
{
    if (task)
    {
        task->FileCopyProgress(pos, size);
    }
}

BuildTask::BuildTask(MainWindow* mainWindow_) : mainWindow(mainWindow_), prevPercent(-1), log(nullptr), package(nullptr), buildCanceled(false)
{
}

void BuildTask::Run()
{
    try
    {
        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        log = mainWindow->GetLog();
        log->Clear();
        log->WriteLine("================ Building Package '" + mainWindow->GetPackage()->Name() + "' ================");
        log->WriteLine("Creating package index...");
        std::unique_ptr<xml::Document> packageDoc = xml::ReadDocument(mainWindow->GetPackage()->FilePath());
        wing_package::PathMatcher pathMatcher(mainWindow->GetPackage()->FilePath());
        std::unique_ptr<wing_package::Package> packagePtr(new wing_package::Package(pathMatcher, packageDoc.get()));
        package = packagePtr.get();
        std::string xmlIndexFilePath = util::Path::ChangeExtension(mainWindow->GetPackage()->FilePath(), ".index.xml");
        package->WriteIndexToXmlFile(xmlIndexFilePath);
        log->WriteLine("==> " + xmlIndexFilePath);
        std::string packageBinFilePath = util::Path::ChangeExtension(mainWindow->GetPackage()->FilePath(), ".bin");
        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        std::string compression = wing_package::CompressionStr(mainWindow->GetPackage()->GetProperties()->GetCompression());
        log->WriteLine("Creating binary package " + packageBinFilePath + ", compression=" + compression + "...");
        package->AddObserver(this);
        package->Create(packageBinFilePath, wing_package::Content::all);
        package->RemoveObserver(this);
        log->WriteLine("==> " + packageBinFilePath);
        log->WriteLine("Creating package info...");
        std::string xmlInfoFilePath = util::Path::ChangeExtension(mainWindow->GetPackage()->FilePath(), ".info.xml");
        package->WriteInfoXmlFile(xmlInfoFilePath);
        log->WriteLine("==> " + xmlInfoFilePath);

        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        log->WriteLine("Making setup programs...");
        BuildLogger buildLogger(log);
        wing_package::MakeSetup(packageBinFilePath, true, &buildLogger);

        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        std::string compileDebugFilePath;
        std::string debugStdoutFilePath;
        std::string debugStderrFilePath;
        std::string compileReleaseFilePath;
        std::string releaseStdoutFilePath;
        std::string releaseStderrFilePath;
        log->WriteLine("Making compile batches..");
        MakeCompileBatches(packageBinFilePath, compileDebugFilePath, debugStdoutFilePath, debugStderrFilePath, compileReleaseFilePath, releaseStdoutFilePath, releaseStderrFilePath);

        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        log->WriteLine("Compiling using debug configuration...");
        Compile(compileDebugFilePath, debugStdoutFilePath, debugStderrFilePath, 0, 50);

        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        log->WriteLine("Compiling using release configuration...");
        Compile(compileReleaseFilePath, releaseStdoutFilePath, releaseStderrFilePath, 50, 100);

        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        std::string binDirectory = util::GetFullPath(util::Path::Combine(util::Path::GetDirectoryName(util::GetFullPath(packageBinFilePath)), "bin"));
        std::string debugSetupFile = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::GetDirectoryName(util::GetFullPath(packageBinFilePath)), "program"), "x64"), "Debug"), "setupd.exe"));
        std::string releaseSetupFile = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::Combine(util::Path::GetDirectoryName(util::GetFullPath(packageBinFilePath)), "program"), "x64"), "Release"), "setup.exe"));
        log->WriteLine("Copying setup files to '" + binDirectory + "' directory...");

        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        Copy(debugSetupFile, binDirectory);

        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        Copy(releaseSetupFile, binDirectory);

        if (buildCanceled)
        {
            throw wing_package::AbortException();
        }
        mainWindow->EndBuild();
        log->WriteLine("================ Package Build Succeeded ================ ");
    }
    catch (const wing_package::AbortException&)
    {
        mainWindow->EndBuild();
        log->WriteLine("Package build canceled");
    }
    catch (const std::exception& ex)
    {
        mainWindow->EndBuild();
        log->WriteLine("error: " + std::string(ex.what()));
    }
}

void BuildTask::Cancel()
{
    buildCanceled = true;
    package->Interrupt();
    package->SetAction(wing_package::Action::abortAction);
}

void BuildTask::MakeCompileBatches(
    const std::string& packageBinFilePath,
    std::string& compileDebugFilePath,
    std::string& debugStdoutFilePath,
    std::string& debugStderrFilePath,
    std::string& compileReleaseFilePath,
    std::string& releaseStdoutFilePath,
    std::string& releaseStderrFilePath)
{
    std::string setupFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(util::GetFullPath(
        packageBinFilePath)), "program"), "setup.sln"));
    std::string compileDirectory = util::GetFullPath(util::Path::Combine(util::Path::GetDirectoryName(util::GetFullPath(packageBinFilePath)), "compile"));
    std::error_code ec;
    std::filesystem::create_directories(compileDirectory, ec);
    if (ec)
    {
        throw std::runtime_error("could not create directory '" + compileDirectory + "': " + util::PlatformStringToUtf8(ec.message()));
    }

    std::unique_ptr<xml::Document> configDoc = wingstall_config::ConfigurationDocument();

    std::string vcvars64BatFilePath = wingstall_config::VCVarsFilePath(configDoc.get());
    if (!std::filesystem::exists(vcvars64BatFilePath))
    {
        throw std::runtime_error("Visual C++ vcvars64.bat file '" + vcvars64BatFilePath + "' does not exist. Check the configuration.");
    }
    std::string boostIncludeDir = wingstall_config::BoostIncludeDir(configDoc.get());
    if (!std::filesystem::exists(boostIncludeDir))
    {
        throw std::runtime_error("Boost include directory '" + boostIncludeDir + "' does not exist. Check the configuration.");
    }
    std::string boostLibDir = wingstall_config::BoostLibDir(configDoc.get());
    if (!std::filesystem::exists(boostLibDir))
    {
        throw std::runtime_error("Boost library directory '" + boostLibDir + "' does not exist. Check the configuration.");
    }

    compileDebugFilePath = util::Path::Combine(compileDirectory, "compile_debug.bat");
    std::ofstream compileDebugFile(compileDebugFilePath);
    util::CodeFormatter debugFormatter(compileDebugFile);
    debugFormatter.WriteLine("call \"" + util::MakeNativePath(vcvars64BatFilePath) + "\"");
    debugStdoutFilePath = util::Path::Combine(compileDirectory, "debug_stdout.txt");
    debugStderrFilePath = util::Path::Combine(compileDirectory, "debug_stderr.txt");
    debugFormatter.WriteLine("msbuild -verbosity:minimal -t:Rebuild -p:Configuration=\"Debug\" \"" +
        util::MakeNativePath(setupFilePath) + "\" > \"" +
        util::MakeNativePath(debugStdoutFilePath) + "\" 2> \"" +
        util::MakeNativePath(debugStderrFilePath) + "\"");

    compileReleaseFilePath = util::Path::Combine(compileDirectory, "compile_release.bat");
    std::ofstream compileReleaseFile(compileReleaseFilePath);
    util::CodeFormatter releaseFormatter(compileReleaseFile);
    releaseFormatter.WriteLine("call \"" + util::MakeNativePath(vcvars64BatFilePath) + "\"");
    releaseStdoutFilePath = util::Path::Combine(compileDirectory, "release_stdout.txt");
    releaseStderrFilePath = util::Path::Combine(compileDirectory, "release_stderr.txt");
    releaseFormatter.WriteLine("msbuild -verbosity:minimal -t:Rebuild -p:Configuration=\"Release\" \"" +
        util::MakeNativePath(setupFilePath) + "\" > \"" +
        util::MakeNativePath(releaseStdoutFilePath) + "\" 2> \"" +
        util::MakeNativePath(releaseStderrFilePath) + "\"");
}

void BuildTask::Compile(const std::string& compileBatFilePath, const std::string& stdOutFilePath, const std::string& stdErrFilePath, int startPercent, int endPercent)
{
    mainWindow->SetBuildProgressPercent(startPercent);
    std::string command = "cmd /C \"" + util::MakeNativePath(compileBatFilePath) + "\"";
    util::Process process(command, util::Process::Redirections::none);
    process.WaitForExit();
    int exitCode = process.ExitCode();
    if (std::filesystem::exists(stdOutFilePath))
    {
        std::string stdoutFileStr = util::ReadFile(stdOutFilePath);
        if (!stdoutFileStr.empty())
        {
            log->WriteLine(stdoutFileStr);
        }
    }
    if (std::filesystem::exists(stdErrFilePath))
    {
        std::string stderrFileStr = util::ReadFile(stdErrFilePath);
        if (!stderrFileStr.empty())
        {
            log->WriteLine(stderrFileStr);
        }
    }
    if (exitCode != 0)
    {
        throw std::runtime_error("compile command returned exit code " + std::to_string(exitCode));
    }
    mainWindow->SetBuildProgressPercent(endPercent);
}

void BuildTask::Copy(const std::string& setupFilePath, const std::string& binDirectory)
{
    BuildCopyFileObserver observer(log, this);
    prevPercent = -1;
    std::string dest = util::Path::Combine(binDirectory, util::Path::GetFileName(setupFilePath));
    CopyFile(setupFilePath, dest, true, true, true, &observer);
}

void BuildTask::StatusChanged(wing_package::Package* package)
{
    if (log)
    {
        log->WriteLine(package->GetStatusStr());
        if (package->GetStatus() == wing_package::Status::failed)
        {
            log->WriteLine(package->GetErrorMessage());
        }
    }
}

void BuildTask::LogError(wing_package::Package* package, const std::string& error)
{
    if (log)
    {
        log->WriteLine(package->GetErrorMessage());
        log->WriteLine("Package Build Failed");
    }
}

void BuildTask::FileContentPositionChanged(wing_package::Package* package)
{
    int percent = 0;
    if (package->FileContentPosition() == package->FileContentSize())
    {
        percent = 100;
    }
    else
    {
        percent = static_cast<int>((100.0 * package->FileContentPosition()) / package->FileContentSize());
    }
    if (percent != prevPercent)
    {
        mainWindow->SetBuildProgressPercent(percent);
        prevPercent = percent;
    }
}

void BuildTask::FileCopyProgress(int64_t pos, int64_t size)
{
    int percent = 0;
    if (pos == size)
    {
        percent = 100;
    }
    else
    {
        percent = static_cast<int>((100.0 * pos) / size);
    }
    if (percent != prevPercent)
    {
        mainWindow->SetBuildProgressPercent(percent);
        prevPercent = percent;
    }
}

void RunBuildTask(BuildTask* buildTask)
{
    try
    {
        buildTask->Run();
    }
    catch (...)
    {
    }
}

} // package_editor

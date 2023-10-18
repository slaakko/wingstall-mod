// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.shell;

import std.core;

export namespace wing {

void ShellInit();
void ShellDone();
std::string GetProgramFilesDirectoryPath();
std::string GetStartMenuProgramsFolderPath();
std::string GetDesktopFolderPath();
void GetShellLinkData(const std::string& linkFilePath, std::string& path, std::string& arguments, std::string& workingDirectory,
    std::string& description, std::string& iconPath, int& iconIndex);
void CreateShellLink(const std::string& linkFilePath, const std::string& path, const std::string& arguments, const std::string& workingDirectory,
    const std::string& description, const std::string& iconPath, int iconIndex);

} // wing

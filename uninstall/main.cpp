// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <Windows.h>

import wing_gui;
import wing_package;
import wing;
import xpath;
import util;
import std.core;

void InitApplication(HINSTANCE instance)
{
    util::Init();
    wing::Init(instance);
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    try
    {
        InitApplication(instance);
        std::string currentExecutablePath = util::GetFullPath(util::GetPathToExecutable());
        std::string uninstallPackageFilePath = util::Path::Combine(util::Path::GetDirectoryName(currentExecutablePath), "uninstall.bin");
        std::string commandLine(cmdLine);
        if (!commandLine.empty())
        {
            uninstallPackageFilePath = util::GetFullPath(commandLine);
        }
        wing_package::Package package;
        package.ReadIndex(uninstallPackageFilePath);
        wing_package::SetInfoItem(wing_package::InfoItemKind::appName, new wing_package::StringItem(package.AppName()));
        wing_package::SetInfoItem(wing_package::InfoItemKind::appVersion, new wing_package::StringItem(package.Version()));
        wing::Icon& setupIcon = wing::Application::GetResourceManager().GetIcon("setup_icon");
        wing_gui::UninstallWindow uninstallWindow;
        uninstallWindow.SetPackage(&package);
        uninstallWindow.SetIcon(setupIcon);
        uninstallWindow.SetSmallIcon(setupIcon);
        wing::Application::Run(uninstallWindow);
    }
    catch (const std::exception& ex)
    {
        wing::ShowMessageBox(nullptr, "Uninstall error", ex.what());
        return 1;
    }
    return 0;
}

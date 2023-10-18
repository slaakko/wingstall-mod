// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <Windows.h>

import package_editor.main_window;
import package_editor.configuration;
import wing;
import util;
import xpath;
import std.filesystem;

void InitApplication(HINSTANCE instance)
{
    util::Init();
    wing::Init(instance);
    package_editor::InitConfiguration();
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    try
    {
        InitApplication(instance);
        package_editor::MainWindow mainWindow;
        wing::Icon& icon = wing::Application::GetResourceManager().GetIcon("package.icon");
        mainWindow.SetIcon(icon);
        mainWindow.SetSmallIcon(icon);
        wing::Application::Run(mainWindow);
    }
    catch (const std::exception& ex)
    {
        wing::ShowMessageBox(nullptr, "error", ex.what());
        return 1;
    }
    return 0;
}

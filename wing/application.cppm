// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module wing.application;

import wing.keys;
import wing.base;
import wing.resource_manager;
import wing.window_manager;
import std.core;

export namespace wing {

class Window;

using KeyPreviewFn = std::function<void(Keys, bool&)>;

class KeyPreviewMethod
{
public:
    KeyPreviewMethod()
    {
    }
    template<class T>
    void SetHandlerFunction(T* t, void (T::* pm)(Keys, bool&))
    {
        fn = std::bind(pm, t, std::placeholders::_1, std::placeholders::_2);
    }
    void operator()(Keys key, bool& handled)
    {
        fn(key, handled);
    }
private:
    KeyPreviewFn fn;
};

class LogView;

class Application
{
public:
    static int Run(Window& mainWindow);
    static void Exit();
    static void Exit(int exitCode);
    static ResourceManager& GetResourceManager() { return resourceManager; }
    static WindowManager& GetWindowManager() { return windowManager; }
    static void SetMainWindow(Window* mainWindow_) { mainWindow = mainWindow_; }
    static Window* MainWindow() { return mainWindow; }
    static void SetActiveWindow(Window* activeWindow_) { activeWindow = activeWindow_; }
    static Window* ActiveWindow() { return activeWindow; }
    static void ProcessMessages();
    static void SetLogView(LogView* logView_) { logView = logView_; }
    static LogView* GetLogView() { return logView; }
private:
    static bool ProcessMessage(HWND handle, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result, void*& originalWndProc);
    static void ModelessWindowKeyPreview(Keys key, bool& handled);
    friend void ApplicationInit();
    static void Init();
    static WindowManager windowManager;
    static ResourceManager resourceManager;
    static Window* mainWindow;
    static Window* activeWindow;
    static LogView* logView;
};

void ApplicationInit();
void ApplicationDone();

} // wing

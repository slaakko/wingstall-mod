// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing.window_manager;

import wing.control;
import wing.base;
import util;

namespace wing {

WindowManager::WindowManager()
{
    systemClassNames.insert("BUTTON");
    systemClassNames.insert("STATIC");
    systemClassNames.insert("EDIT");
    systemClassNames.insert("LISTBOX");
    systemClassNames.insert("COMBOBOX");
}

bool WindowManager::IsSystemClassName(const std::string& windowClassName) const
{
    auto it = systemClassNames.find(windowClassName);
    return it != systemClassNames.cend();
}

uint16_t WindowManager::Register(const std::string& windowClassName, uint32_t windowClassStyle, int windowClassBackgroundColor)
{
    auto it = registeredWindowClasses.find(windowClassName);
    if (it != registeredWindowClasses.cend())
    {
        return it->second;
    }
    std::u16string className = util::ToUtf16(windowClassName);
    HINSTANCE instance = Instance();
    WNDCLASSEXW wc;
    wc.cbSize = sizeof(wc);
    wc.style = windowClassStyle;
    wc.lpfnWndProc = GetWndProc();
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = nullptr;
    int64_t c = windowClassBackgroundColor + 1;
    HBRUSH bgc = *reinterpret_cast<HBRUSH*>(&c);
    wc.hbrBackground = bgc;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = (LPWSTR)className.c_str();
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    uint16_t windowClass = RegisterClassExW(&wc);
    if (windowClass == 0)
    {
        throw WindowsException(GetLastError());
    }
    registeredWindowClasses[windowClassName] = windowClass;
    return windowClass;
}

void WindowManager::AddWindow(Control* window)
{
    windowMap[window->Handle()] = window;
}

void WindowManager::RemoveWindow(Control* window)
{
    windowMap.erase(window->Handle());
}

Control* WindowManager::GetWindow(HWND handle) const
{
    auto it = windowMap.find(handle);
    if (it != windowMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

} // wing

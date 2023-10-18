// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

export module wing.base;

import wing.keys;
import std.core;

export namespace wing {

class WindowsException : public std::runtime_error
{
public:
    WindowsException(uint64_t errorCode_);
    uint64_t ErrorCode() const { return errorCode; }
    std::string ErrorMessage() const { return what(); }
private:
    uint64_t errorCode;
};

using MessageProcessorFunction = bool(*)(HWND handle, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result, void*& originalWndProc);

struct Message
{
    Message(HWND handle_, UINT message_, WPARAM wParam_, LPARAM lParam_, LRESULT result_) :
        handle(handle_), message(message_), wParam(wParam_), lParam(lParam_), result(result_), originalWndProc(nullptr)
    {
    }
    uint32_t LParamLoDWord() const { return static_cast<uint32_t>(lParam); }
    uint32_t LParamHiDWord() const { return static_cast<uint32_t>((lParam >> 32) & 0xFFFFFFFF); }
    uint16_t WParamLoWord() const { return static_cast<uint16_t>(wParam & 0xFFFF); }
    uint16_t WParamHiWord() const { return static_cast<uint16_t>((wParam >> 16) & 0xFFFF); }
    HWND handle;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    LRESULT result;
    void* originalWndProc;
};

void SetMessageProcessorFunction(MessageProcessorFunction messageProcessorFun);

struct Padding
{
    Padding() : left(0), top(0), right(0), bottom(0) {}
    Padding(int left_, int top_, int right_, int bottom_) : left(left_), top(top_), right(right_), bottom(bottom_) {}
    int Vertical() const { return top + bottom; }
    int Horizontal() const { return left + right; }
    int left;
    int top;
    int right;
    int bottom;
};

inline bool operator==(const Padding& left, const Padding& right)
{
    return left.left == right.left && left.top == right.top && left.right == right.right && left.bottom == right.bottom;
}

inline bool operator!=(const Padding& left, const Padding& right)
{
    return !(left == right);
}

enum class KeyState : int
{
    none = 0, shift = 1 << 0, control = 1 << 1, alt = 1 << 2
};

inline KeyState operator|(KeyState left, KeyState right)
{
    return KeyState(int(left) | int(right));
}

inline KeyState operator&(KeyState left, KeyState right)
{
    return KeyState(int(left) & int(right));
}

inline KeyState operator~(KeyState state)
{
    return KeyState(~int(state));
}

bool KeyPressed(int virtualKeyCode);

KeyState GetKeyState();

using KeyPreviewFunction = void (*)(Keys key, bool& handled);

void SetKeyPreviewFunction(KeyPreviewFunction keyPreviewFun);
Keys MakeKeyWithState(Keys key);

HINSTANCE Instance();
WNDPROC GetWndProc();

int Run();
int MessageLoop();

using DialogResultFunction = int (*)(void* dialogWindowPtr);
using DialogWindowKeyPreviewFunction = void (*)(void* dialogWindowPtr, Keys key, bool& handled);


int DialogMessageLoop(HWND handle, HWND parentHandle, DialogResultFunction dialogResultFn, DialogWindowKeyPreviewFunction dialogWindowKeyPreviewFn, void* dialogWindowPtr);

void WingInit(HINSTANCE instance_);
void WingDone();

using Color = Gdiplus::Color;

Color GetSystemColor(int index);

void ShowMessageBox(HWND handle, const std::string& caption, const std::string& message);
void ShowInfoMessageBox(HWND handle, const std::string& message);
void ShowErrorMessageBox(HWND handle, const std::string& message);

LRESULT CALLBACK CommandSubClassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

std::string SelectDirectory(HWND handle, const std::string& directoryPath);

} // wing

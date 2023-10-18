// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing.manual_reset_event;

namespace wing {

ManualResetEvent::ManualResetEvent() : handle(nullptr)
{
    handle = CreateEventW(nullptr, true, false, nullptr);
}

ManualResetEvent::~ManualResetEvent()
{
    if (handle != nullptr)
    {
        bool retval = CloseHandle(handle);
    }
}

void ManualResetEvent::Reset()
{
    bool retval = ResetEvent(handle);
}

void ManualResetEvent::Set()
{
    bool retval = SetEvent(handle);
}

void ManualResetEvent::WaitFor()
{
    DWORD retval = WaitForSingleObject(handle, INFINITE);
}

} // wing

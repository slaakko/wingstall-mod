// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing.dialog;

import wing.base;
import util;

namespace wing {

bool OpenFileName(HWND windowHandle, const std::vector<std::pair<std::string, std::string>>& descriptionFilterPairs, const std::string& initialDirectory,
    const std::string& defaultFilePath, const std::string& defaultExtension,
    uint32_t flags, std::string& firstPath, std::string& currentDirectory, std::vector<std::string>& fileNames)
{
    int filterBufferSize = 0;
    for (const auto& descFilter : descriptionFilterPairs)
    {
        std::u16string desc = util::ToUtf16(descFilter.first);
        std::u16string filter = util::ToUtf16(descFilter.second);
        filterBufferSize += desc.length() + 1 + filter.length() + 1;
    }
    filterBufferSize += 2;
    std::unique_ptr<char16_t[]> filterBuffer(new char16_t[filterBufferSize]);
    char16_t* filterBufferPtr = filterBuffer.get();
    for (const auto& descFilter : descriptionFilterPairs)
    {
        std::u16string desc = util::ToUtf16(descFilter.first);
        std::memcpy(filterBufferPtr, desc.c_str(), (desc.length() + 1) * sizeof(char16_t));
        filterBufferPtr += desc.length() + 1;
        std::u16string filter = util::ToUtf16(descFilter.second);
        std::memcpy(filterBufferPtr, filter.c_str(), (filter.length() + 1) * sizeof(char16_t));
        filterBufferPtr += filter.length() + 1;
    }
    *filterBufferPtr++ = u'\0';
    *filterBufferPtr++ = u'\0';
    DWORD fileNameBufferSize = 16 * 1024;
    std::unique_ptr<char16_t[]> fileNameBuffer(new char16_t[fileNameBufferSize]);
    if (defaultFilePath.empty())
    {
        fileNameBuffer[0] = u'\0';
    }
    else
    {
        std::u16string defFilePath = util::ToUtf16(util::MakeNativePath(defaultFilePath));
        std::memcpy(fileNameBuffer.get(), defFilePath.c_str(), (defFilePath.length() + 1) * sizeof(char16_t));
    }
    std::u16string initialDir = util::ToUtf16(util::MakeNativePath(initialDirectory));
    std::u16string defaultExt = util::ToUtf16(defaultExtension);
    OPENFILENAMEW openFileName;
    std::memset(&openFileName, 0, sizeof(openFileName));
    openFileName.lStructSize = sizeof(openFileName);
    openFileName.hwndOwner = (HWND)windowHandle;
    openFileName.hInstance = Instance();
    openFileName.lpstrFilter = (LPCWSTR)filterBuffer.get();
    openFileName.lpstrCustomFilter = nullptr;
    openFileName.nMaxCustFilter = 0;
    openFileName.nFilterIndex = 0;
    openFileName.lpstrFile = reinterpret_cast<LPWSTR>(fileNameBuffer.get());
    openFileName.nMaxFile = fileNameBufferSize;
    openFileName.lpstrFileTitle = nullptr;
    openFileName.nMaxFileTitle = 0;
    openFileName.lpstrInitialDir = (LPCWSTR)initialDir.c_str();
    openFileName.lpstrTitle = nullptr;
    openFileName.Flags = flags;
    openFileName.nFileOffset = 0;
    openFileName.nFileExtension = 0;
    openFileName.lpstrDefExt = (LPCWSTR)defaultExt.c_str();
    openFileName.lCustData = 0;
    openFileName.lpfnHook = nullptr;
    openFileName.lpTemplateName = nullptr;
    openFileName.pvReserved = nullptr;
    openFileName.dwReserved = 0;
    openFileName.FlagsEx = 0;
    bool succeeded = GetOpenFileNameW(&openFileName);
    if (!succeeded)
    {
        return false;
    }
    if ((flags & OFN_ALLOWMULTISELECT) == 0)
    {
        firstPath = util::ToUtf8(fileNameBuffer.get());
        currentDirectory = util::Path::GetDirectoryName(firstPath);
        fileNames.push_back(util::Path::GetFileName(firstPath));
    }
    else
    {
        firstPath = util::ToUtf8(fileNameBuffer.get());
        currentDirectory = firstPath;
        const char16_t* p = fileNameBuffer.get();
        size_t len = wcslen((const wchar_t*)p);
        p = p + len + 1;
        while (*p)
        {
            len = wcslen((const wchar_t*)p);
            std::string path = util::ToUtf8(std::u16string(p, len));
            fileNames.push_back(path);
            p = p + len + 1;
        }
    }
    return succeeded;
}

} // wing

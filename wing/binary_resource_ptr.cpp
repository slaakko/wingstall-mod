// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing.binary_resource_ptr;

import wing.base;
import util;

namespace wing {

    BinaryResourcePtr::BinaryResourcePtr(const std::string& moduleName, const std::string& resourceName_) : resourceName(resourceName_), size(0)
    {
        std::u16string moduleNameStr = util::ToUtf16(moduleName);
        std::u16string resourceNameStr = util::ToUtf16(resourceName);
        HMODULE moduleHandle = GetModuleHandleW((LPCWSTR)moduleNameStr.c_str());
        if (!moduleHandle)
        {
            throw WindowsException(GetLastError());
        }
        HRSRC res = FindResourceW(moduleHandle, (LPCWSTR)resourceNameStr.c_str(), RT_RCDATA);
        if (!res)
        {
            throw WindowsException(GetLastError());
        }
        HGLOBAL handle = LoadResource(nullptr, res);
        if (!handle)
        {
            throw WindowsException(GetLastError());
        }
        data = static_cast<uint8_t*>(LockResource(handle));
        size = SizeofResource(nullptr, res);
    }

} // wing

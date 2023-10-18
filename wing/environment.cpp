// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing.environment;

import wing.registry;
import wing.base;
import util;

namespace wing {

bool HasSystemEnvironmentVariable(const std::string& name)
{
    RegistryKey key = RegistryKey::Open(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Control\\Session Manager\\Environment", KEY_READ);
    return key.HasValue(name);
}

std::string GetSystemEnvironmentVariable(const std::string& name)
{
    RegistryKey key = RegistryKey::Open(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Control\\Session Manager\\Environment", KEY_READ);
    return key.GetValue(name);
}

void SetSystemEnvironmentVariable(const std::string& name, const std::string& value, RegistryValueKind valueKind)
{
    RegistryKey key = RegistryKey::Open(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Control\\Session Manager\\Environment", KEY_WRITE);
    key.SetValue(name, value, valueKind);
}

void DeleteSystemEnvironmentVariable(const std::string& name)
{
    RegistryKey key = RegistryKey::Open(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Control\\Session Manager\\Environment", KEY_ALL_ACCESS);
    key.DeleteValue(name);
}

bool HasPathDirectory(const std::string& directory)
{
    if (HasSystemEnvironmentVariable("Path"))
    {
        std::string path = GetSystemEnvironmentVariable("Path");
        std::string dirPath = util::ToLowerNarrow(util::GetFullPath(directory));
        std::vector<std::string> directories = util::Split(path, ';');
        int n = directories.size();
        for (int i = 0; i < n; ++i)
        {
            if (util::ToLowerNarrow(util::GetFullPath(directories[i])) == dirPath)
            {
                return true;
            }
        }
    }
    return false;
}

void AppendPathDirectory(const std::string& directory)
{
    if (HasSystemEnvironmentVariable("Path"))
    {
        std::string path = GetSystemEnvironmentVariable("Path");
        std::vector<std::string> directories = util::Split(path, ';');
        directories.push_back(directory);
        std::string newPath;
        bool first = true;
        for (const std::string& directory : directories)
        {
            if (!directory.empty())
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    newPath.append(1, ';');
                }
                newPath.append(directory);
            }
        }
        SetSystemEnvironmentVariable("Path", newPath, RegistryValueKind::regExpandSz);
    }
    else
    {
        throw std::runtime_error("'Path' environment variable not found");
    }
}

void RemovePathDirectory(const std::string& directory)
{
    if (HasSystemEnvironmentVariable("Path"))
    {
        std::string path = GetSystemEnvironmentVariable("Path");
        std::vector<std::string> directories = util::Split(path, ';');
        std::vector<std::string> newDirectories;
        std::string newPath;
        std::string dirPath = util::ToLowerNarrow(util::GetFullPath(directory));
        for (const std::string& dir : directories)
        {
            if (!dir.empty())
            {
                if (util::ToLowerNarrow(util::GetFullPath(dir)) != dirPath)
                {
                    newDirectories.push_back(dir);
                }
            }
        }
        bool first = true;
        for (const std::string& dir : newDirectories)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                newPath.append(1, ';');
            }
            newPath.append(dir);
        }
        SetSystemEnvironmentVariable("Path", newPath, RegistryValueKind::regExpandSz);
    }
    else
    {
        throw std::runtime_error("'Path' environment variable not found");
    }
}

void BroadcastEnvironmentChangedMessage()
{
    const char* environment = "Environment";
    DWORD flags = BSF_ALLOWSFW | BSF_FORCEIFHUNG | BSF_IGNORECURRENTTASK | BSF_NOHANG;
    DWORD info = BSM_ALLCOMPONENTS | BSM_ALLDESKTOPS | BSM_APPLICATIONS;
    UINT action = 0;
    long retval = BroadcastSystemMessageA(flags, &info, WM_SETTINGCHANGE, action, (LPARAM)environment);
    if (retval <= 0)
    {
        throw WindowsException(GetLastError());
    }
}

} // wing

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.path;

import std.core;
import std.filesystem;

export namespace util {

class InvalidPathException : public std::runtime_error
{
public:
    InvalidPathException(const std::string& message_);
};

std::string GetCurrentWorkingDirectory();
bool FileExists(const std::string& filePath);
bool DirectoryExists(const std::string& directoryPath);
bool PathExists(const std::string& path);
std::string GetFullPath(const std::string& path);
std::string MakeRelativeDirPath(const std::string& dirPath, const std::string& referenceDirPath);

class Path
{
public:
    static std::string MakeCanonical(const std::string& path);
    static std::string ChangeExtension(const std::string& path, const std::string& extension);
    static bool HasExtension(const std::string& path);
    static std::string GetExtension(const std::string& path);
    static std::string GetDrive(const std::string& path);
    static std::string GetFileName(const std::string& path);
    static std::string GetFileNameWithoutExtension(const std::string& path);
    static std::string GetDirectoryName(const std::string& path);
    static std::string Combine(const std::string& path1, const std::string& path2);
    static bool IsAbsolute(const std::string& path);
    static bool IsRelative(const std::string& path);
};

std::string MakeNativePath(const std::string& path);
std::filesystem::path MakeNativeFileSystemPath(const std::string& path);

} // namespace util

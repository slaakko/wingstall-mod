// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import util;
import std.filesystem;
import std.core;

void InitApplication()
{
    util::Init();
}

bool FilesEqual(const std::string& leftFilePath, const std::string& rightFilePath)
{
    if (!std::filesystem::exists(leftFilePath))
    {
        std::cout << "left file '" << leftFilePath << "' does not exist" << std::endl;
        return false;
    }
    if (!std::filesystem::exists(rightFilePath))
    {
        std::cout << "right file '" << rightFilePath << "' does not exist" << std::endl;
        return false;
    }
    uintmax_t leftFileSize = std::filesystem::file_size(leftFilePath);
    uintmax_t rightFileSize = std::filesystem::file_size(rightFilePath);
    if (leftFileSize != rightFileSize)
    {
        std::cout << "file '" << leftFilePath << "' and '" << rightFilePath << "' sizes differ (" << leftFileSize << " != " << rightFileSize << ")" << std::endl;
        return false;
    }
    util::FileStream leftFile(leftFilePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream leftBufStream(leftFile);
    util::FileStream rightFile(rightFilePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream rightBufStream(rightFile);
    int64_t n = leftFileSize;
    for (int64_t i = 0; i < n; ++i)
    {
        int lx = leftBufStream.ReadByte();
        if (lx == -1)
        {
            std::cout << "unexpected end of file '" << leftFilePath << std::endl;
            return false;
        }
        int rx = rightBufStream.ReadByte();
        if (rx == -1)
        {
            std::cout << "unexpected end of file '" << rightFilePath << std::endl;
            return false;
        }
        if (lx != rx)
        {
            std::cout << "file '" << leftFilePath << "' and '" << rightFilePath << "' contents differ (byte " << i << ": " << lx << " != " << rx << ")" << std::endl;
            return false;
        }
    }
    return true;
}

void CompareDirectories(const std::string& leftDir, const std::string& rightDir, int& numDifferentDirectories, int& numDifferentFiles)
{
    bool leftExists = true;
    if (!std::filesystem::exists(leftDir))
    {
        std::cout << "left directory '" << leftDir << "' does not exist" << std::endl;
        ++numDifferentDirectories;
        leftExists = false;
    }
    if (!std::filesystem::exists(rightDir))
    {
        std::cout << "right directory '" << rightDir << "' does not exist" << std::endl;
        ++numDifferentDirectories;
    }
    if (leftExists)
    {
        std::error_code ec;
        std::filesystem::directory_iterator it(leftDir, ec);
        if (ec)
        {
            throw std::runtime_error("could not iterate directory '" + leftDir + "': " + util::PlatformStringToUtf8(ec.message()));
        }
        while (it != std::filesystem::directory_iterator())
        {
            if (it->path().filename() != "." && it->path().filename() != "..")
            {
                std::string name = it->path().filename().generic_string();
                if (std::filesystem::is_directory(it->path()))
                {
                    CompareDirectories(util::Path::Combine(leftDir, name), util::Path::Combine(rightDir, name), numDifferentDirectories, numDifferentFiles);
                }
                else if (std::filesystem::is_regular_file(it->path()))
                {
                    if (!FilesEqual(util::Path::Combine(leftDir, name), util::Path::Combine(rightDir, name)))
                    {
                        ++numDifferentFiles;
                    }
                }
            }
            ++it;
        }
    }
}


int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        std::string left;
        std::string right;
        if (argc <= 2)
        {
            throw std::runtime_error("usage: filecompare LEFT-PATH RIGHT-PATH");
        }
        for (int i = 1; i < argc; ++i)
        {
            if (i == 1)
            {
                left = util::GetFullPath(argv[i]);
            }
            else if (i == 2)
            {
                right = util::GetFullPath(argv[i]);
            }
        }
        if (!std::filesystem::exists(left))
        {
            std::cout << "left path '" << left << "' does not exist" << std::endl;
            return 1;
        }
        if (!std::filesystem::exists(right))
        {
            std::cout << "right path '" << right << "' does not exist" << std::endl;
            return 1;
        }
        std::filesystem::file_status leftStatus = std::filesystem::status(left);
        std::filesystem::file_status rightStatus = std::filesystem::status(right);
        if (leftStatus.type() == std::filesystem::file_type::regular &&
            rightStatus.type() == std::filesystem::file_type::regular)
        {
            if (FilesEqual(left, right))
            {
                std::cout << "files '" << left << "' and '" << right << "' are equal" << std::endl;
            }
        }
        else if (leftStatus.type() == std::filesystem::file_type::directory &&
            rightStatus.type() == std::filesystem::file_type::directory)
        {
            int numDifferentDirectories = 0;
            int numDifferentFiles = 0;
            CompareDirectories(left, right, numDifferentDirectories, numDifferentFiles);
            if (numDifferentDirectories == 0 && numDifferentFiles == 0)
            {
                std::cout << "all equal" << std::endl;
            }
            else
            {
                std::cout << numDifferentDirectories << " directories differ" << std::endl;
                std::cout << numDifferentFiles << " files differ" << std::endl;
            }
        }
        else
        {
            throw std::runtime_error("both arguments must be regular files or directories");
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

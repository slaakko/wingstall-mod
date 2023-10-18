// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.file_util;

import std.core;

export namespace util {

class CopyFileObserver
{
public:
    virtual void WriteLine(const std::string& line) {}
    virtual void FileCopyProgress(int64_t pos, int64_t size) {}
};

class CoutCopyFileObserver : public CopyFileObserver
{
public:
    void WriteLine(const std::string& line) override;
};

void CopyFile(const std::string& source, const std::string& dest, bool force, bool makeDir, bool verbose);
void CopyFile(const std::string& source, const std::string& dest, bool force, bool makeDir, bool verbose, CopyFileObserver* observer);

} // namespace util

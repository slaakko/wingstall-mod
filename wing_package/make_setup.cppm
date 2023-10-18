// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.make_setup;

import std.core;

export namespace wing_package {

class Logger
{
public:
    virtual void WriteLine(const std::string& line) = 0;
};

class CoutLogger : public Logger
{
public:
    void WriteLine(const std::string& line) override;
};

void MakeSetup(const std::string& packageBinFilePath, bool verbose);
void MakeSetup(const std::string& packageBinFilePath, bool verbose, Logger* logger);

} // namespace wing_package

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.log;

import std.core;

export namespace util {

enum class LogMode
{
    console, queue
};

void SetLogMode(LogMode mode);
void StartLog();
void EndLog();
void LogMessage(int logStreamId, const std::string& message);
void LogMessage(int logStreamId, const std::string& message, int indent);
int WaitForLogMessage();
int FetchLogMessage(char16_t* buf, int size);
std::string FetchLogMessage(bool& endOfLog, int timeoutMs, bool& timeout);

} // namespace util

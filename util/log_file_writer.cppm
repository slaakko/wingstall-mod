// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.log_file_writer;

import std.core;

export namespace util {

class LogFileWriter
{
public:
    LogFileWriter(const std::string& logFilePath_, bool open_);
    LogFileWriter(const std::string& logFilePath);
    void WriteCurrentDateTime();
    void WriteLine();
    void WriteLine(const std::string& line);
    void Write(const std::string& s);
    typedef std::basic_ostream<char, std::char_traits<char>> CoutType;
    typedef CoutType& (*StandardEndLine)(CoutType&);
    LogFileWriter& operator<<(StandardEndLine manip);
    std::ostream& LogFile() { return logFile; }
private:
    bool open;
    std::string logFilePath;
    std::ofstream logFile;
};

LogFileWriter& operator<<(LogFileWriter& writer, const std::string& s);
LogFileWriter& operator<<(LogFileWriter& writer, const char* s);
LogFileWriter& operator<<(LogFileWriter& writer, char c);
LogFileWriter& operator<<(LogFileWriter& writer, bool b);
LogFileWriter& operator<<(LogFileWriter& writer, int x);
LogFileWriter& operator<<(LogFileWriter& writer, double x);
LogFileWriter& operator<<(LogFileWriter& writer, int64_t x);
LogFileWriter& operator<<(LogFileWriter& writer, uint64_t x);

} // namespace util

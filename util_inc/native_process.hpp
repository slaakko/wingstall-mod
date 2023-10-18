// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_NATIVE_PROCESS_INCLUDED
#define UTIL_NATIVE_PROCESS_INCLUDED
#include <string>
#include <memory>

namespace util_inc {

class NativeProcessImpl;

class NativeProcess
{
public:
    enum class Redirections : int
    {
        none = 0,
        processStdIn = 1 << 0,
        processStdOut = 1 << 1,
        processStdErr = 1 << 2
    };
    enum class StdHandle : int
    {
        stdOut = 1, stdErr = 2
    };
    NativeProcess(const std::string& command, Redirections redirections);
    ~NativeProcess();
    bool Running();
    void WaitForExit();
    int ExitCode() const;
    void Terminate();
    bool Eof(StdHandle handle);
    std::string ReadLine(StdHandle handle);
    std::string ReadToEnd(StdHandle handle);
    void WriteLine(const std::string& line);
private:
    NativeProcessImpl* impl;
};

constexpr NativeProcess::Redirections operator|(NativeProcess::Redirections left, NativeProcess::Redirections right)
{
    return NativeProcess::Redirections(int(left) | int(right));
}

constexpr NativeProcess::Redirections operator&(NativeProcess::Redirections left, NativeProcess::Redirections right)
{
    return NativeProcess::Redirections(int(left) & int(right));
}

int GetNativePid();

} // namespace util_inc

#endif // UTIL_NATIVE_PROCESS_INCLUDED

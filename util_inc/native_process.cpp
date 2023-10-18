// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <util_inc/native_process.hpp>
#include <util_inc/text_util.hpp>
#include <boost/process.hpp>
#ifdef _WIN32
#include <boost/process/windows.hpp>
#include <Windows.h>
#undef min
#undef max
#else
#include <sys/types.h>
#include <unistd.h>
#endif

namespace util_inc {

class NativeProcessImpl
{
public:
    NativeProcessImpl(const std::string& command, NativeProcess::Redirections redirections_);
    NativeProcessImpl(const NativeProcessImpl&) = delete;
    NativeProcessImpl& operator=(const NativeProcessImpl&) = delete;
    NativeProcessImpl(NativeProcessImpl&&) = delete;
    NativeProcessImpl& operator=(NativeProcessImpl&&) = delete;
    bool Running();
    void WaitForExit();
    int ExitCode() const;
    void Terminate();
    bool Eof(NativeProcess::StdHandle handle);
    std::string ReadLine(NativeProcess::StdHandle handle);
    std::string ReadToEnd(NativeProcess::StdHandle handle);
    void WriteLine(const std::string& line);
private:
    NativeProcess::Redirections redirections;
    boost::process::ipstream processStdOut;
    boost::process::ipstream processStdErr;
    boost::process::opstream processStdIn;
    boost::process::child process;
    bool stdOutEof;
    bool stdErrEof;
};

NativeProcessImpl::NativeProcessImpl(const std::string& command, NativeProcess::Redirections redirections_) : redirections(redirections_), stdOutEof(false), stdErrEof(false)
{
    try
    {
        switch (redirections)
        {
        case NativeProcess::Redirections::none:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::windows::hide);
#else
            process = boost::process::child(command);
#endif
            break;
        }
        case NativeProcess::Redirections::processStdIn:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_in < processStdIn);
#endif
            break;
        }
        case NativeProcess::Redirections::processStdOut:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_out > processStdOut, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_out > processStdOut);
#endif
            break;
        }
        case NativeProcess::Redirections::processStdErr:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_err > processStdErr, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_err > processStdErr);
#endif
            break;
        }
        case NativeProcess::Redirections::processStdIn | NativeProcess::Redirections::processStdOut:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_out > processStdOut, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_out > processStdOut);
#endif
            break;
        }
        case NativeProcess::Redirections::processStdIn | NativeProcess::Redirections::processStdErr:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_err > processStdErr, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_err > processStdErr);
#endif
            break;
        }
        case NativeProcess::Redirections::processStdOut | NativeProcess::Redirections::processStdErr:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_out > processStdOut, boost::process::std_err > processStdErr, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_out > processStdOut, boost::process::std_err > processStdErr);
#endif
            break;
        }
        case NativeProcess::Redirections::processStdIn | NativeProcess::Redirections::processStdOut | NativeProcess::Redirections::processStdErr:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_out > processStdOut, boost::process::std_err > processStdErr,
                boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_out > processStdOut, boost::process::std_err > processStdErr);
#endif
            break;
        }
        }
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("error running command '" + command + "': " + PlatformStringToUtf8(ex.what()));
    }
}

bool NativeProcessImpl::Running()
{
    return process.running();
}

void NativeProcessImpl::WaitForExit()
{
    process.wait();
}

int NativeProcessImpl::ExitCode() const
{
    return process.exit_code();
}

void NativeProcessImpl::Terminate()
{
    process.terminate();
}

bool NativeProcessImpl::Eof(NativeProcess::StdHandle handle)
{
    switch (handle)
    {
    case NativeProcess::StdHandle::stdOut:
    {
        if ((redirections & NativeProcess::Redirections::processStdOut) != NativeProcess::Redirections::none)
        {
            if (!stdOutEof && processStdOut) return false;
        }
        break;
    }
    case NativeProcess::StdHandle::stdErr:
    {
        if ((redirections & NativeProcess::Redirections::processStdErr) != NativeProcess::Redirections::none)
        {
            if (!stdErrEof && processStdErr) return false;
        }
        break;
    }
    }
    return true;
}

std::string NativeProcessImpl::ReadLine(NativeProcess::StdHandle handle)
{
    std::string line;
    switch (handle)
    {
    case NativeProcess::StdHandle::stdOut:
    {
        if ((redirections & NativeProcess::Redirections::processStdOut) != NativeProcess::Redirections::none)
        {
            if (processStdOut)
            {
                if (!std::getline(processStdOut, line))
                {
                    stdOutEof = true;
                }
            }
        }
        break;
    }
    case NativeProcess::StdHandle::stdErr:
    {
        if ((redirections & NativeProcess::Redirections::processStdErr) != NativeProcess::Redirections::none)
        {
            if (processStdErr)
            {
                if (!std::getline(processStdErr, line))
                {
                    stdErrEof = true;
                }
            }
        }
        break;
    }
    }
    return line;
}

std::string NativeProcessImpl::ReadToEnd(NativeProcess::StdHandle handle)
{
    std::string lines;
    bool first = true;
    while (!Eof(handle))
    {
        if (first)
        {
            first = false;
        }
        else
        {
            lines.append(1, '\n');
        }
        std::string line = ReadLine(handle);
        if (!line.empty())
        {
            lines.append(line);
        }
    }
    return lines;
}

void NativeProcessImpl::WriteLine(const std::string& line)
{
    if ((redirections & NativeProcess::Redirections::processStdIn) != NativeProcess::Redirections::none)
    {
        if (processStdIn)
        {
            processStdIn << line << std::endl;
        }
    }
    else
    {
        throw std::runtime_error("process stdin not redirected");
    }
}

NativeProcess::NativeProcess(const std::string& command, Redirections redirections) : impl(new NativeProcessImpl(command, redirections))
{
}

NativeProcess::~NativeProcess()
{
    delete impl;
}

bool NativeProcess::Running()
{
    return impl->Running();
}

void NativeProcess::WaitForExit()
{
    impl->WaitForExit();
}

int NativeProcess::ExitCode() const
{
    return impl->ExitCode();
}

void NativeProcess::Terminate()
{
    impl->Terminate();
}

bool NativeProcess::Eof(StdHandle handle)
{
    return impl->Eof(handle);
}

std::string NativeProcess::ReadLine(StdHandle handle)
{
    return impl->ReadLine(handle);
}

std::string NativeProcess::ReadToEnd(StdHandle handle)
{
    return impl->ReadToEnd(handle);
}

void NativeProcess::WriteLine(const std::string& line)
{
    impl->WriteLine(line);
}

int GetNativePid()
{
#ifdef _WIN32
    return GetCurrentProcessId();
#else
    return getpid();
#endif
}

} // namespace util_inc

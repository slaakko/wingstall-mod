// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module wing_gui.status_message;

import wing_package;
import wing;
import std.core;

export namespace wing_gui {

const int STATUS_MESSAGE_AVAILABLE = WM_USER + 2;

enum class StatusMessageKind : int
{
    statusChanged, componentChanged, fileChanged, streamPositionChanged, fileIndexChanged, logError
};

class StatusMessage
{
public:
    StatusMessage(StatusMessageKind kind_);
    virtual ~StatusMessage();
    StatusMessageKind Kind() const { return kind; }
private:
    StatusMessageKind kind;
};

class StatusChangedMessage : public StatusMessage
{
public:
    StatusChangedMessage(const wing_package::Status& status_, const std::string& statusStr_, const std::string& errorMessage_);
    wing_package::Status GetStatus() const { return status; }
    const std::string& StatusStr() const { return statusStr; }
    const std::string& ErrorMessage() const { return errorMessage; }
private:
    wing_package::Status status;
    std::string statusStr;
    std::string errorMessage;
};

class ComponentChangedMessage : public StatusMessage
{
public:
    ComponentChangedMessage(const std::string& component_);
    const std::string& Component() const { return component; }
private:
    std::string component;
};

class FileChangedMessage : public StatusMessage
{
public:
    FileChangedMessage(const std::string& file_);
    const std::string& File() const { return file; }
private:
    std::string file;
};

class StreamPositionChangedMessage : public StatusMessage
{
public:
    StreamPositionChangedMessage(int64_t position_);
    int64_t Position() const { return position; }
private:
    int64_t position;
};

class FileIndexChangedMessage : public StatusMessage
{
public:
    FileIndexChangedMessage(int fileIndex_);
    int FileIndex() const { return fileIndex; }
private:
    int fileIndex;
};

class LogErrorMessage : public StatusMessage
{
public:
    LogErrorMessage(const std::string& error_);
    const std::string& Error() const { return error; }
private:
    std::string error;
};

class StatusMessageQueue
{
public:
    StatusMessageQueue();
    void Put(StatusMessage* message);
    std::unique_ptr<StatusMessage> Get();
private:
    std::mutex mtx;
    std::queue<std::unique_ptr<StatusMessage>> q;
};

} // namespace wing_gui


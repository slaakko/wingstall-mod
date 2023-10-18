// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.socket;

import std.core;

export namespace util {

enum class ShutdownMode
{
    receive = 0, send = 1, both = 2
};

int64_t CreateSocket();
void BindSocket(int64_t socketHandle, int port);
void ListenSocket(int64_t socketHandle, int backLog);
int64_t AcceptSocket(int64_t socketHandle);
void CloseSocket(int64_t socketHandle);
void ShutdownSocket(int64_t socketHandle, ShutdownMode mode);
int64_t ConnectSocket(const std::string& node, const std::string& service);
int SendSocket(int64_t socketHandle, uint8_t* buf, int len, int flags);
int ReceiveSocket(int64_t socketHandle, uint8_t* buf, int len, int flags);
void InitSocket();
void DoneSocket();

class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(const std::string& node, const std::string& service);
    TcpSocket(int64_t handle_) noexcept;
    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;
    TcpSocket(TcpSocket&& that) noexcept;
    TcpSocket& operator=(TcpSocket&& that) noexcept;
    ~TcpSocket();
    void Close();
    void Connect(const std::string& node, const std::string& service);
    void Bind(int port);
    void Listen(int backlog);
    TcpSocket Accept();
    void Shutdown(ShutdownMode mode);
    void Send(const uint8_t* buffer, int count);
    int Receive(uint8_t* buffer, int count);
private:
    int64_t handle;
    bool connected;
    bool shutdown;
};

void Write(TcpSocket& socket, const std::string& s);
std::string ReadStr(TcpSocket& socket);

} // namespace util

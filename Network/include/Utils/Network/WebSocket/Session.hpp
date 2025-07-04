#pragma once

#include <shared_mutex>

#include "BasicType.hpp"
#include "Utils/MessageQueue.hpp"

using namespace Utils::MessageQueue;

namespace Utils :: HTTP {

namespace WebSocket {


    class Session : public std::enable_shared_from_this<Session>{
    public:
        using SessionPtr = std::shared_ptr<Session>;
        using MessageHandler = std::function<void(SessionPtr, const std::string&)>;

        Session(TCPSocket&& socket);

        TCPSocket& GetSocket();                      // 获取socket
        int& GetSessionId();
        int GetPort();
        std::string GetAddress();

        void Start();
        void Close();
        void SentData(const std::string& data);

        void SetMessageHandler(MessageHandler messageHandler);

    private:
        Beast::websocket::stream<TCPSocket> ws_;
        Asio::strand<IOContext::executor_type> strand_;
        Beast::flat_buffer buffer_;
        int sessionId_;
        std::deque<std::string> writeQueue_;
        MessageHandler messageHandler_;

        void OnAccept(const Beast::error_code& ec);
        void DoRead();
        void OnRead(const Beast::error_code& ec, std::size_t bytes_transferred);
        void DoWrite();
        void OnWrite(const Beast::error_code& ec, std::size_t bytes_transferred);
    };

    using SessionPtr = Session::SessionPtr;

    class SessionManager {
    public:
        int AddSession(SessionPtr sessionPtr);
        void RemoveSession(int sessionId);
        void CleanExpiredSession();

        void Unicast(int sessionId, const std::string& data);
        void Broadcast(const std::string& data);
        int SessionCount() const;
    private:
        mutable std::shared_mutex sessionMutex_;
        std::unordered_map<int, Session::SessionPtr> sessions_;
        std::atomic<int> sessionId_;
    };

    using SessionManagerPtr = std::shared_ptr<SessionManager>;

}

}


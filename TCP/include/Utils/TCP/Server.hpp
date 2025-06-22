#pragma once

#include "Session.hpp"
#include "Acceptor.hpp"

namespace Utils :: TCP {

    class TCPServer {
    public:
        TCPServer(int port = 3030, std::string endOfMessage = "\n", int maxSessionCount = 64,
               size_t ioThreadCount =(std::thread::hardware_concurrency() + 1) / 2
               );
        ~TCPServer();
        void Start();
        void Stop();

        void Broadcast(const std::string& data);
        void Unicast(int sessionId, const std::string& data);
        void CloseSession(int sessionId);
        void GetMessage(TCPMessage& message);

    private:
        int port_;
        int maxSessionCount_;
        std::atomic<bool> isRunning_;
        std::string endOfMessage_;

        IOContextPoolPtr ioContextPoolPtr_;
        AcceptorPtr acceptorPtr_;
        SessionManager sessionManager_;
        MessageQueuePtr messageQueuePtr_;

        void handleNewSession(TCPSocketPtr socketPtr);
    };

}
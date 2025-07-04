#pragma once
#include "Session.hpp"

namespace Utils :: HTTP {

namespace WebSocket {

    class WebSocketServer {
    public:
        WebSocketServer(int port);

        void Start();
        void Stop();

        void Broadcast(const std::string& data);
        void Unicast(int sessionId, const std::string& data);
        void CloseSession(int sessionId);
        void GetMessage(WebSocketMessage& message);

    private:
        int port_;
        std::atomic<bool> isRunning_;

        IOContext ioContext_;
        TCPAcceptor acceptor_;
        SessionManager sessionManager_;
        WebSocketMessageQueue messageQueue_;

        void DoAccept();

    };

}
}
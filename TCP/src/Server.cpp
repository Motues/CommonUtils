#include "Utils/TCP/Server.hpp"

namespace Utils :: TCP {

    TCPServer::TCPServer(int port, std::string endOfMessage, int maxSessionCount, size_t ioThreadCount) :
        port_(port), endOfMessage_(std::move(endOfMessage)), maxSessionCount_(maxSessionCount) , isRunning_(false){

        TCPEndPoint endPoint(boost::asio::ip::tcp::v4(), port);
        ioContextPoolPtr_ = std::make_shared<IOContextPool>(ioThreadCount);
        acceptorPtr_ = std::make_shared<Acceptor>(*ioContextPoolPtr_, endPoint);
        messageQueuePtr_ = std::make_shared<TCPMessageQueue>();
    }

    TCPServer::~TCPServer() {
        Stop();
    }

    void TCPServer::Start() {
        isRunning_ = true;
        ioContextPoolPtr_->Start();
        acceptorPtr_->Start([&](TCPSocketPtr socketPtr) {
            handleNewSession(socketPtr);
        });
    }

    void TCPServer::Stop() {
        isRunning_ = false;
        acceptorPtr_->Stop();
        ioContextPoolPtr_->Stop();
    }

    void TCPServer::handleNewSession(TCPSocketPtr socketPtr) {
        auto session = std::make_shared<Session>(*socketPtr, endOfMessage_);
        session->SetMessageHandler([this](SessionPtr sessionPtr, const std::string & data) {
            TCPMessage message;
            message.data = data;
            message.ip = sessionPtr->GetAddress();
            message.port = sessionPtr->GetPort();
            message.time = SystemClock::now();
            message.sessionId = sessionPtr->GetSessionId();
            messageQueuePtr_->push(message);
        });
        sessionManager_.AddSession(session);
        session->Start();
    }

    void TCPServer::Broadcast(const std::string& data) {
        sessionManager_.Broadcast(data);
    }

    void TCPServer::Unicast(int sessionId, const std::string& data) {
        sessionManager_.Unicast(sessionId, data);
    }

    void TCPServer::CloseSession(int sessionId) {
        sessionManager_.RemoveSession(sessionId);
    }

    void TCPServer::GetMessage(TCPMessage& message) {
        message = messageQueuePtr_->pop();
    }


}
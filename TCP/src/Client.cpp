#include "Utils/TCP/Client.hpp"

namespace Utils::TCP {


TCPClient::TCPClient(std::string serverAddress, int port, std::string endOfMessage) :
    serverAddress_(std::move(serverAddress)), port_(port) , endOfMessage_(std::move(endOfMessage)){}

TCPClient::~TCPClient() {
    CloseConnection();
}

bool TCPClient::ConnectToServer() {
    TCPEndPoint endpoint(boost::asio::ip::make_address(serverAddress_), port_);
    TCPSocket socket(ioContext_);
    boost::system::error_code ec;
    socket.connect(endpoint, ec);
    if (ec) return false;

    sessionPtr_ = std::make_shared<Session>(socket, endOfMessage_);
    sessionPtr_->SetMessageHandler([this](SessionPtr session, const std::string& data) {
        recDataQueue_.push(data);
    });
    sessionPtr_->Start();

    // 启动 IO 上下文
    std::thread t([this] { ioContext_.run(); });
    t.detach();

    return true;
}


bool TCPClient::SendData(const std::string& data) {
    sessionPtr_->SentData(data);
    return true;
}

bool TCPClient::RecData(std::string& data) {
    if (recDataQueue_.empty()) {
        return false;
    }
    data = recDataQueue_.pop();
    return true;
}

bool TCPClient::CloseConnection() {
    sessionPtr_->Close();
    return true;
}

}
#pragma once

#include <memory>
#include <iostream>

#include "BasicType.hpp"
#include "Session.hpp"

namespace Utils::TCP {

class TCPClient {
public:
    TCPClient(std::string serverAddress = "127.0.0.1", int port = 3030);
    ~TCPClient();

    bool ConnectToServer();
    bool SendData(const std::string& data);
    bool RecData(std::string& data);
    bool CloseConnection();

private:
    std::string serverAddress_;
    int port_;
    IOContext ioContext_;
    SessionPtr sessionPtr_;
    MessageHandler  messageHandler_;

    MessageQueue<std::string> recDataQueue_;
};

}
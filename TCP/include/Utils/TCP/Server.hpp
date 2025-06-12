#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <iostream>

#include "BasicType.hpp"
#include "Utils/MessageQueue.hpp"

namespace Utils :: TCP {

using Utils::MessageQueue::MessageQueue;

class AsyncTCPServer {
public:
    AsyncTCPServer();
    AsyncTCPServer(int port = 1717, std::string serverAddress = "127.0.0.1", IPType ipType = IPType::IPV4);
    ~AsyncTCPServer();

    bool SendData(const std::string& data, TCPClientPtr& clientPtr) const; // 发送数据
    bool RecData(std::string& data, TCPClientPtr& clientPtr); // 接收数据
    bool GetMessage(TCPMessage &message); // 获取服务器接收到的消息

    bool StartServer(); // 启动服务器
    bool CloseServer(); // 关闭服务器

private:
    int port;
    std::string serverAddress;
    IPType ipType;
    int maxClientsNumber{1024};
    IOContextPtr ioContextPtr;
    TCPAcceptorPtr acceptorPtr;
    std::vector<TCPClientPtr> clientsPtr;
    MessageQueue<TCPMessage> messageQueue;
    std::thread serverThread;

    bool CreateServer(); // 创建服务器
    bool BindPort(); // 绑定端口
    bool ListenServer(); // 监听服务器
    bool CloseClient(const TCPClientPtr& clientPtr); // 关闭客户端连接
    void DoAccept();
    void StartRead(const TCPClientPtr& clientPtr);
};

}
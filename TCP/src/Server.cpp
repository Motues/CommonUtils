#include "Utils/TCP/Server.hpp"

namespace Utils::TCP {


#pragma region AsyncTCPServer

AsyncTCPServer::AsyncTCPServer() = default;

AsyncTCPServer::AsyncTCPServer(int port, std::string  serverAddress, IPType ipType) :
    port(port), ipType(ipType), serverAddress(std::move(serverAddress)) {
    if (!CreateServer() || !BindPort()) {
        std::cerr << "Failed to create server" << std::endl;
        return;
    }
}
AsyncTCPServer::~AsyncTCPServer() {
    CloseServer();
    for (auto& clientPtr : clientsPtr) {
        CloseClient(clientPtr);
    }
}

bool AsyncTCPServer::CreateServer() {
    ioContextPtr = std::make_shared<IOContext>();
    acceptorPtr = std::make_shared<TCPAcceptor>(*ioContextPtr);
    if (!ioContextPtr || !acceptorPtr) {
        std::cerr << "Failed to create io_context" << std::endl;
        return false;
    }
    // TODO 用于控制客户端数量
    // clientsPtr.reserve(maxClientsNumber);
    return true;
}
bool AsyncTCPServer::BindPort() {
    TCPEndPoint endpoint;
    if (ipType == IPType::IPV4) {
        endpoint = TCPEndPoint(
            boost::asio::ip::make_address(serverAddress), port);
    } else {
        endpoint = TCPEndPoint(
            boost::asio::ip::make_address_v6(serverAddress), port);
    }
    acceptorPtr->open(endpoint.protocol());
    acceptorPtr->set_option(boost::asio::socket_base::reuse_address(true));
    acceptorPtr->bind(endpoint);

    if (!acceptorPtr->is_open()) {
        std::cerr << "Failed to bind socket" << std::endl;
        return false;
    }
    return true;
}
bool AsyncTCPServer::ListenServer() {
    BoostErrorCode ec;
    acceptorPtr->listen(maxClientsNumber, ec);
    if (ec) {
        std::cerr << "Listen failed: " << ec.message() << std::endl;
        return false;
    }
    return true;
}
bool AsyncTCPServer::StartServer() {
    if(!ListenServer()) {
        std::cerr << "Failed to listen" << std::endl;
        return false;
    }
    DoAccept();
    serverThread = std::thread([this]() {
        ioContextPtr->run();
    });

    return true;
}

//void AsyncTCPServer::StartRead(const TCPClientPtr& clientPtr) {
//    auto buffer = std::make_shared<BoostStreamBuffer>();
//    boost::asio::async_read_until(
//        clientPtr->socket, *buffer, '!',
//        [this, clientPtr, buffer](const BoostErrorCode& ec, std::size_t bytes_transferred) mutable {
//            if (!ec) {
//                std::cout << "Received data from client " << clientPtr->address << ":" << clientPtr->port << std::endl;
//
//                std::string data = std::string(boost::asio::buffer_cast<const char*>(buffer->data()), bytes_transferred);
//                buffer->consume(buffer->size());
//
//                TCPMessage msg;
//                msg.type = MessageType::DATA;
//                msg.data = data;
//                msg.time = std::chrono::system_clock::now();
//                messageQueue.push(msg);
//
//                StartRead(clientPtr); // 递归继续读取
//            } else {
//                std::cerr << "Failed to read data: " << ec.message() << std::endl;
//                CloseClient(clientPtr);
//            }
//        });
//}

void AsyncTCPServer::StartRead(const TCPClientPtr& clientPtr) {
    auto buffer = std::make_shared<std::array<char, 1024>>();
    clientPtr->socket.async_read_some(
        boost::asio::buffer(*buffer),
        [this, clientPtr, buffer](const BoostErrorCode& ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::string data(buffer->data(), bytes_transferred);
                    // 将接收到的数据直接存入消息队列
                    TCPMessage msg;
                    msg.data = data;
                    msg.time = std::chrono::system_clock::now();
                    msg.clientPtr = clientPtr;
                    messageQueue.push(msg);

                    StartRead(clientPtr);
                } else {
                    if (ec != boost::asio::error::eof && ec != boost::asio::error::connection_reset) {
                        std::cerr << "Read error: " << ec.message() << std::endl;
                    }
                    PostCloseClient(clientPtr); // 安全关闭
                    // std::cout<<"Client "<<clientPtr->address<<":"<<clientPtr->port<<" disconnected"<<std::endl;
                }
            });
}



void AsyncTCPServer::DoAccept() {
    if (!acceptorPtr || !acceptorPtr->is_open()) {
        std::cerr << "Server socket not open" << std::endl;
        return;
    }

    // 创建新客户端对象（无需预先绑定 socket）
    TCPClient client(*ioContextPtr);
    TCPClientPtr clientPtr = std::make_shared<TCPClient>(std::move(client));

    // 直接使用异步 accept
    acceptorPtr->async_accept(
        clientPtr->socket,  // 目标 socket
        [this, clientPtr](const BoostErrorCode& ec) {
            if (!ec) {
                // 获取客户端地址和端口（通过 socket 的 remote_endpoint()）
                TCPEndPoint endpoint = clientPtr->socket.remote_endpoint();
                clientPtr->address = endpoint.address().to_string();
                clientPtr->port = endpoint.port();

                clientsPtr.push_back(clientPtr);
                StartRead(clientPtr);
            } else {
                std::cerr << "Failed to accept client: " << ec.message() << std::endl;
            }

            // 继续监听新连接
            DoAccept();
        }
    );
}

bool AsyncTCPServer::SendData(const std::string& data, TCPClientPtr& clientPtr) {
    if (!clientPtr || !clientPtr->socket.is_open()) {
        std::cerr << "Client socket not open" << std::endl;
        return false;
    }
    if (data.empty()) {
        std::cerr << "Data is empty" << std::endl;
        return false;
    }
    BoostErrorCode ec;
    boost::asio::write(clientPtr->socket, boost::asio::buffer(data), ec);
    if (ec) {
        std::cerr << "Failed to send data: " << ec.message() << std::endl;
        PostCloseClient(clientPtr);
        return false;
    }
    return true;
}
bool AsyncTCPServer::RecData(std::string& data, TCPClientPtr& clientPtr) {
    if (!clientPtr || !clientPtr->socket.is_open()) {
        std::cerr << "Client socket not open" << std::endl;
        return false;
    }
    BoostStreamBuffer buffer;
    BoostErrorCode ec;
    std::size_t bytes_transferred = boost::asio::read(clientPtr->socket, buffer,
                                                      boost::asio::transfer_at_least(1), ec);
    if (ec == boost::asio::error::eof) {
        std::cerr << "Server disconnected." << std::endl;
        return false;
    }
    if (ec) {
        std::cerr << "Failed to receive data: " << ec.message() << std::endl;
        return false;
    }
    const boost::asio::const_buffer data_buffer = buffer.data();
    data = std::string(static_cast<const char*>(data_buffer.data()), bytes_transferred);
//    data = std::string(boost::asio::buffer_cast<const char*>(buffer.data()), bytes_transferred);
    return true;
}

bool AsyncTCPServer::GetMessage(TCPMessage &message) {
    // MessageQueue 是线程安全的，自动处理容量为空的情况
    if (messageQueue.empty()) {
        return false;
    }
    message = messageQueue.pop();
    return true;
}

bool AsyncTCPServer::CloseServer() {
    if (acceptorPtr && acceptorPtr->is_open()) {
        acceptorPtr->close();
    }
    if (ioContextPtr) {
        ioContextPtr->stop();
    }
    return true;
}

bool AsyncTCPServer::CloseClient(const TCPClientPtr& clientPtr) {
    if (!clientPtr || !clientPtr->socket.is_open()) {
        return true; // 已关闭或无效指针，直接返回成功
    }
    BoostErrorCode ec;
    // 先尝试 shutdown，并忽略错误
    clientPtr->socket.shutdown(TCPSocket::shutdown_both, ec);
    if (ec) {
        // 忽略特定错误，如 "not connected"
        std::cerr << "Shutdown error: " << ec.message() << std::endl;
    }
    // 再关闭 socket
    clientPtr->socket.close(ec);
    if (ec) {
        std::cerr << "Close error: " << ec.message() << std::endl;
    }
    // 从客户端列表移除
    std::lock_guard<std::mutex> lock(clientsMutex);
    clientsPtr.erase(
        std::remove(clientsPtr.begin(), clientsPtr.end(), clientPtr),
        clientsPtr.end()
    );
    return true;
}
void AsyncTCPServer::PostCloseClient(const TCPClientPtr& clientPtr) {
    if (!clientPtr) return;

    ioContextPtr->post([this, clientPtr]() {
        CloseClient(clientPtr);  // 确保在IO线程执行关闭
    });
}

#pragma region AsyncTCPServer

}

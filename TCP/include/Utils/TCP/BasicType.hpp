#pragma once

#include <chrono>

#include <boost/asio.hpp>

namespace Utils :: TCP {
    using IOContext = boost::asio::io_context;
    using IOContextPtr = std::shared_ptr<IOContext>;
    using BoostErrorCode = boost::system::error_code;
    using BoostStreamBuffer = boost::asio::streambuf;
    using BoostConstBuffer = boost::asio::const_buffer;
    using BoostMutableBuffer = boost::asio::mutable_buffer;

    enum class IPType{
        IPV4 = 0,
        IPV6 = 1
    };

    using TCPAcceptor = boost::asio::ip::tcp::acceptor;
    using TCPSocket = boost::asio::ip::tcp::socket;
    using TCPResolver = boost::asio::ip::tcp::resolver;
    using TCPEndPoint = boost::asio::ip::tcp::endpoint;

    using TCPAcceptorPtr = std::shared_ptr<TCPAcceptor>;
    using TCPSocketPtr = std::shared_ptr<TCPSocket>;

    // TCP服务器用于记录客户端信息
    struct TCPClient {
        TCPSocket socket;
        std::string address;
        int port{0};

        explicit TCPClient(IOContext &ioContext) :
            socket(TCPSocket (ioContext)) {}
    };
    using TCPClientPtr = std::shared_ptr<TCPClient>;

    // TCP消息
    using Time = std::chrono::system_clock::time_point;
    struct TCPMessage {
        Time time;
        TCPClientPtr clientPtr;
        std::string data;
    };
    using TCPMessagePtr = std::shared_ptr<TCPMessage>;

}


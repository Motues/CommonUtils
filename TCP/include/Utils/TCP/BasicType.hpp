#pragma once

#include <chrono>

#include <boost/asio.hpp>
#include <Utils/MessageQueue.hpp>

namespace Utils :: TCP {

    using MessageQueue::MessageQueue;

    using IOContext = boost::asio::io_context;
    using IOContextPtr = std::shared_ptr<IOContext>;
    using BoostErrorCode = boost::system::error_code;
    using BoostStreamBuffer = boost::asio::streambuf;
    using BoostConstBuffer = boost::asio::const_buffer;
    using BoostMutableBuffer = boost::asio::mutable_buffer;

    using TCPAcceptor = boost::asio::ip::tcp::acceptor;
    using TCPSocket = boost::asio::ip::tcp::socket;
    using TCPResolver = boost::asio::ip::tcp::resolver;
    using TCPEndPoint = boost::asio::ip::tcp::endpoint;

    using TCPAcceptorPtr = std::shared_ptr<TCPAcceptor>;
    using TCPSocketPtr = std::shared_ptr<TCPSocket>;


    // TCP消息
    using SystemClock = std::chrono::system_clock;
    using TimePoint = SystemClock::time_point;
    using Milliseconds = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;
    struct TCPMessage {
        TimePoint time;
        int sessionId;
        int port;
        std::string ip;
        std::string data;
    };
    using TCPMessagePtr = std::shared_ptr<TCPMessage>;
    using TCPMessageQueue = MessageQueue<TCPMessage>;
    using MessageQueuePtr = std::shared_ptr<TCPMessageQueue>;

    enum class IPType {
        IPV4,
        IPV6
    };

}


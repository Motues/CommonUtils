#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/url.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <chrono>

#include "Utils/MessageQueue.hpp"


namespace Utils :: Network {

    using MessageQueue::MessageQueue;

    namespace Beast = boost::beast;
    namespace HTTP = Beast::http;
    namespace URL = boost::urls;
    namespace Asio = boost::asio;
    namespace SSL = boost::asio::ssl;

    using BoostErrorCode = boost::system::error_code;

    using IOContext = boost::asio::io_context;
    using TCPAcceptor = boost::asio::ip::tcp::acceptor;
    using TCPSocket = boost::asio::ip::tcp::socket;
    using TCPResolver = boost::asio::ip::tcp::resolver;
    using TCPEndPoint = boost::asio::ip::tcp::endpoint;

    using IOContextPtr = std::shared_ptr<IOContext>;
    using TCPAcceptorPtr = std::shared_ptr<TCPAcceptor>;
    using TCPSocketPtr = std::shared_ptr<TCPSocket>;

    using SystemTime = std::chrono::system_clock;


}
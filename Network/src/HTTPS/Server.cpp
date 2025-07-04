#include "Utils/Network/HTTPS/Server.hpp"

namespace Utils :: Network {
namespace HTTPS {

    HTTPServer::HTTPServer(int port) : port_(port) , isRunning_(false){
        ioContextPtr_ = std::make_shared<IOContext>();
        acceptorPtr_ = std::make_shared<TCPAcceptor>(*ioContextPtr_, TCPEndPoint(Asio::ip::tcp::v4(), port_));
        workGuardPtr_ = std::make_shared<WorkGuard>(ioContextPtr_->get_executor());
    }

    void HTTPServer::Start() {
        isRunning_ = true;
        ioThread_ = std::thread([this]() {
            ioContextPtr_->run();
        });
        DoAccept();
    }

    void HTTPServer::Stop() {
        isRunning_ = false;
        ioContextPtr_->stop();
        if (ioThread_.joinable()) {
            ioThread_.join();
        }
    }

    void HTTPServer::AddRoute(const std::string &path, const std::string &method,
                              HTTPRequestHandler handler) {
        router_.AddRoute(method, path, handler);
    }

    void HTTPServer::DoAccept() {
        acceptorPtr_->async_accept(
            *ioContextPtr_,
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!ec) {
                    // 创建 Session 并启动
                    auto session = std::make_shared<Session>(std::move(socket));
                    session->SetRouter(router_);
                    session->Start();
                } else {
                    std::cerr << "Accept error: " << ec.message() << "\n";
                }
                if(isRunning_) {
                    // 继续等待下一个连接
                    DoAccept();
                }

            });
    }

}
}
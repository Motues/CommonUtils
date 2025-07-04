#include "Utils/Network/HTTPS/Session.hpp"

namespace Utils :: Network {

namespace HTTPS {

    Session::Session(TCPSocket&& socket)
        : socket_(std::forward<TCPSocket>(socket)) {}
    Session::Session(IOContext& ioContext)
        : socket_(ioContext) {}

    void Session::SetRouter(Router& router) {
        routerPtr_ = &router;
    }

    void Session::Start() {
        DoRead();
    }

    void Session::Close() {
        socket_.close();
    }

    void Session::DoRead() {
        auto self(shared_from_this());
        HTTP::async_read(
            socket_, buffer_, request_,
            [self](Beast::error_code ec, std::size_t /*bytes_transferred*/) {
                if (!ec) {
                    self->RequestHandler();
                } else {
                    // 忽略 EOF（关闭连接）
                    if (ec != HTTP::error::end_of_stream)
                    self->Close();
                }
            });
    }

    void Session::RequestHandler() {
        auto response = std::make_shared<HTTPResponseString>(HTTP::status::ok, request_.version());
        // response.set(HTTP::field::server, "C++/Network");
        // response.set(HTTP::field::content_type, "text/plain");
        if(routerPtr_) {
            routerPtr_->HandleRequest(request_, *response);
        } else {
            response->body() = "No handler registered.";
            response->result(HTTP::status::internal_server_error);
        }

        // 自动设置 Content-Length
        response->prepare_payload();

        // 异步发送响应
        auto self(shared_from_this());
        HTTP::async_write(
            socket_, *response,
            [self, response](Beast::error_code ec, std::size_t /*bytes_transferred*/) {
                    if (ec) {
                        std::cerr << "Write error: " << ec.message() << "\n";
                    }
                    self->Close();
                    return;
            });
    }
}

}
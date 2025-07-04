#include "Utils/Network/Request.hpp"
#include "iostream"

namespace Utils :: Network ::Request {

    using namespace boost::urls;


    SyncHttpClient::SyncHttpClient() : ctx_(SSL::context::tlsv12_client) {
        // 设置SSL上下文
        ctx_.set_default_verify_paths();
        ctx_.set_verify_mode(SSL::verify_peer);
    }

    Response SyncHttpClient::Request(
        const std::string &method, const std::string &host,
        const std::string &port, const std::string &target,
        const std::string &body = "", const Headers &headers = {}) {

        if (port == "443") {
            return HTTPSRequest(method, host, port, target, body, headers);
        } else {
            return HTTPRequest(method, host, port, target, body, headers);
        }
    }

    Response SyncHttpClient::HTTPRequest(
        const std::string &method, const std::string &host,
        const std::string &port, const std::string &target,
        const std::string &body, const Headers &headers) {

        Response response;
        try {
            IOContext ioc; // I/O上下文
            TCPResolver resolver(ioc); // 解析器查找端点
            Beast::tcp_stream stream(ioc);

            auto const results = resolver.resolve(host, port); // 查找域名
            stream.connect(results); // 建立连接

            // 设置HTTP请求
            HTTPRequestString req;
            req.method_string(method);
            req.method_string(method);
            req.target(target);
            req.version(11); // HTTP/1.1
            req.set(HTTP::field::host, host);
            req.set(HTTP::field::user_agent, BOOST_BEAST_VERSION_STRING);

            // 添加自定义头部
            for (const auto &[key, value]: headers) {
                req.set(key, value);
            }

            // 设置请求体
            if (!body.empty()) {
                req.body() = body;
                req.prepare_payload();
            }


            HTTP::write(stream, req); // 发送HTTP请求

            // 接收HTTP响应
            Beast::flat_buffer buffer;
            HTTPResponseString res;
            // HTTP::response<HTTP::dynamic_body> res; // 创建动态体
            res.body().reserve(1024);
            HTTP::read(stream, buffer, res);

            // 填充响应结构
            response.status = res.result_int();
            response.text = res.body().data();
            // response.text = Beast::buffers_to_string(res.body().data());

            // 转换头部
            for (const auto &field: res) {
                response.headers[std::string(field.name_string())] = std::string(field.value());
            }

            // 关闭socket
            Beast::error_code ec;
            stream.socket().shutdown(TCPSocket::shutdown_both, ec);

            // 不关心关闭错误
            if (ec && ec != Beast::errc::not_connected) {
                throw Beast::system_error{ec};
            }
        }
        catch (std::exception const &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            response.status = -1;
            response.text = e.what();
        }

        return response;
    }

    Response SyncHttpClient::HTTPSRequest(
        const std::string &method, const std::string &host,
        const std::string &port, const std::string &target,
        const std::string &body, const Request::Headers &headers) {
        Response response;

        try {
            IOContext ioc; // I/O上下文

            // SSL流和解析器
            TCPResolver resolver(ioc);
            SSL::stream<TCPSocket> stream(ioc, ctx_);

            // 设置SNI主机名（重要！）
            if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
                Beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                     boost::asio::error::get_ssl_category()};
                throw Beast::system_error{ec};
            }

            auto const results = resolver.resolve(host, port); // 查找域名

            // 建立TCP连接
            auto &socket = Beast::get_lowest_layer(stream);
            socket.connect(results->endpoint());

            // 执行SSL握手
            stream.handshake(SSL::stream_base::client);

            // 设置HTTP请求
            HTTPRequestString req;
            req.method_string(method);
            req.target(target);
            req.version(11); // HTTP/1.1
            req.set(HTTP::field::host, host);
            req.set(HTTP::field::user_agent, BOOST_BEAST_VERSION_STRING);

            // 添加自定义头部
            for (const auto &[key, value]: headers) {
                req.set(key, value);
            }

            // 设置请求体
            if (!body.empty()) {
                req.body() = body;
                req.prepare_payload();
            }

            HTTP::write(stream, req); // 发送HTTP请求

            // 接收HTTP响应
            Beast::flat_buffer buffer;
            HTTPResponseString res;
            // HTTP::response<HTTP::dynamic_body> res; // 创建动态体
            res.body().reserve(1024);
            HTTP::read(stream, buffer, res);

            // 填充响应结构
            response.status = res.result_int();
            response.text = res.body().data();
            // response.text = Beast::buffers_to_string(res.body().data());

            // 转换头部
            for (const auto &field: res) {
                response.headers[std::string(field.name_string())] = std::string(field.value());
            }

            // 关闭SSL流
            Beast::error_code ec;
            stream.shutdown(ec);

            // 不关心关闭错误
            if (ec == boost::asio::error::eof || ec == SSL::error::stream_truncated) {
                ec = {};
            }
            if (ec) {
                throw Beast::system_error{ec};
            }
        }
        catch (std::exception const &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            response.status = -1;
            response.text = e.what();
        }
        return response;
    }


    // 辅助函数 - GET请求
    Response Get(const std::string &url, const Headers &headers = {}) {
        SyncHttpClient client;
        try {
            UrlParser parser(url);
            return client.Request("GET", parser.host, parser.port, parser.target, "", headers);
        } catch (const std::exception &e) {
            return Response{-1, e.what()};
        }
    }

    // 辅助函数 - POST请求
    Response Post(const std::string &url, const std::string &body, const Headers &headers = {}) {
        SyncHttpClient client;

        try {
            UrlParser parser(url);
            return client.Request("POST", parser.host, parser.port, parser.target, body, headers);
        } catch (const std::exception &e) {
            return Response{-1, e.what()};
        }
    }
}

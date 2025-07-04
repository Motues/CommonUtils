#include <utility>

#include "Utils/Network/HTTPS/Router.hpp"

namespace Utils :: Network {

namespace HTTPS {

    void Router::AddRoute(const std::string &method, const std::string &path, HTTPRequestHandler handler) {
        std::string key = method + ":" + path;
        routes_[key] = std::move(handler); // 覆盖
    }

    HTTPRequestHandler Router::GetRoute(std::string &method, std::string &path) {
        std::string key = method + ":" + path;
        auto it = routes_.find(key);
        if (it != routes_.end()) {
            return it->second;
        }
        return nullptr;
    }

    void Router::HandleRequest(HTTPRequestString& req, HTTPResponseString& res) {
        std::string method = req.method_string().data();
        std::string path = req.target().data();

        std::string key = method + ":" + path;

        // 查找对应方法下的所有路由
        auto method_it = routes_.find(key);
        if (method_it == routes_.end()) {
            NotFoundHandler(res);
            return;
        }
        HTTPRequestHandler& handler = method_it->second;
        handler(req, res);
    }

    void Router::NotFoundHandler(HTTPResponseString &res) {
        res.result(HTTP::status::not_found);
        res.set(HTTP::field::content_type, "text/plain");
        res.body() = "404 Not Found";
        res.prepare_payload();
    }
}

}

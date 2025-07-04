#pragma once

#include "../BasicType.hpp"

namespace Utils :: Network {
namespace HTTPS {



    // response 的状态码和数据，由 Router 统一处理
    class Router {
    public:
        void AddRoute(const std::string& method, const std::string& path, HTTPRequestHandler handler);
        HTTPRequestHandler GetRoute(std::string& method, std::string& path);
        void HandleRequest(HTTPRequestString& req, HTTPResponseString& res);

    private:
        std::unordered_map<std::string, HTTPRequestHandler> routes_;
        static void NotFoundHandler(HTTPResponseString& res); // 404
    };
    using RouterPtr = std::shared_ptr<Router>;
}

}

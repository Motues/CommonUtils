#include "Utils/Network/Request/Post.hpp"
#include "iostream"

namespace Utils :: Network {
namespace Request {

    using namespace boost::urls;

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
}

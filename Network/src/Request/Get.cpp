#include "Utils/Network/Request/Get.hpp"
#include "iostream"

namespace Utils :: Network {
namespace Request {

    using namespace boost::urls;

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

}
}

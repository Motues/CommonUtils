#pragma once

#include "BasicType.hpp"
#include "UrlParser.hpp"
#include <map>

namespace Utils :: HTTP {
    namespace Request {
        using Headers = std::map<std::string, std::string>;
        struct Response {
            int status;
            std::string text;
            Headers  headers;
        };

        Response Get(const std::string& url, const Headers& headers);
        Response Post(const std::string& url, const std::string& body, const Headers& headers);

        class SyncHttpClient {
        public:
            SyncHttpClient();
            Response Request(
                const std::string& method,
                const std::string& host,
                const std::string& port,
                const std::string& target,
                const std::string& body,
                const Headers& headers);
        private:
            SSL::context ctx_;
            Response HTTPRequest(
                const std::string& method,
                const std::string& host,
                const std::string& port,
                const std::string& target,
                const std::string& body,
                const Headers& headers);
            Response HTTPSRequest(
                const std::string& method,
                const std::string& host,
                const std::string& port,
                const std::string& target,
                const std::string& body,
                const Headers& headers);
        };

        struct
    }
}
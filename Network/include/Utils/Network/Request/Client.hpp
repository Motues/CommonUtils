#pragma once

#include "BasicType.hpp"
#include "../UrlParser.hpp"

namespace Utils :: Network {
    namespace Request {

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

    }
}
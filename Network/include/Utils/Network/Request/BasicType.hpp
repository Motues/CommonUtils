#pragma once

#include "../BasicType.hpp"
#include <map>

namespace Utils :: Network {
namespace Request {

    using HTTPRequestString = HTTP::request<HTTP::string_body>;
    using HTTPResponseString = HTTP::response<HTTP::string_body>;

    using Headers = std::map<std::string, std::string>;
    struct Response {
        int status;
        std::string text;
        Headers headers;
    };
    }
}
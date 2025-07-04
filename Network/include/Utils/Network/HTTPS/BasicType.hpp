#pragma once

#include "../BasicType.hpp"

namespace Utils :: Network {
namespace HTTPS {

    using HTTPRequestString = HTTP::request<HTTP::string_body>;
    using HTTPResponseString = HTTP::response<HTTP::string_body>;
    using HTTPRequestHandler = std::function<void(HTTPRequestString&, HTTPResponseString&)>;

}
}
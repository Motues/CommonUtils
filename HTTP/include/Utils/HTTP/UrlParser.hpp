#pragma once

#include <string>

namespace Utils :: HTTP {

struct UrlParser {
    explicit UrlParser(const std::string &url);

    std::string protocol;
    std::string host;
    std::string port;
    std::string target;
};

}

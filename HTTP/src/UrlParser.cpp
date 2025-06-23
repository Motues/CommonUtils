#include "Utils/HTTP/UrlParser.hpp"

#include <stdexcept>

namespace Utils::HTTP {

UrlParser::UrlParser(const std::string &url) {
    size_t protocol_end = url.find("://");
    if (protocol_end == std::string::npos) {
        throw std::runtime_error("Invalid URL: missing protocol");
    }

    protocol = url.substr(0, protocol_end);
    std::string rest = url.substr(protocol_end + 3);

    size_t host_end = rest.find('/');
    if (host_end == std::string::npos) {
        host = rest;
        target = "/";
    } else {
        host = rest.substr(0, host_end);
        target = rest.substr(host_end);
    }

    size_t port_start = host.find(':');
    if (port_start != std::string::npos) {
        port = host.substr(port_start + 1);
        host = host.substr(0, port_start);
    } else {
        port = (protocol == "https") ? "443" : "80";
    }
}


}

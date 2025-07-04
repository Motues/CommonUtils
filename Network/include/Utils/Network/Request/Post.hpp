#pragma once

#include "Client.hpp"

namespace Utils :: Network {
namespace Request {

    Response Post(const std::string& url, const std::string& body, const Headers& headers);
}
}
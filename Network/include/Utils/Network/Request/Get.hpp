#pragma once

#include "Client.hpp"

namespace Utils :: Network {
namespace Request {

    Response Get(const std::string& url, const Headers& headers);
}
}
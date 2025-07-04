#include <iostream>
#include "Utils/Network/Request.hpp"


int main() {

    using namespace Utils::Network::Request;

    Response response = Get("https://www.baidu.com/",  {});
    std::cout << "Status: " << response.status << std::endl;
    std::cout << "Headers:" << std::endl;
    for (const auto& [key, value] : response.headers) {
        std::cout << " " << key << ": " << value << std::endl;
    }
    std::cout << "Body length: " << response.text.size() << " bytes" << std::endl;
    std::cout << "Body: " << response.text << std::endl;
}

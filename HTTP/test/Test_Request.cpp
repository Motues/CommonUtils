#include "Utils/HTTP.hpp"
#include <iostream>


int main() {

    using namespace Utils::HTTP::Request;

    Response response = Get("https://restapi.amap.com/v3/geocode/regeo?key=3f97f3dce8b62b205f6f25dc2830e942&location=114.4266997,30.5166103",  {});
    std::cout << "Status: " << response.status << std::endl;
    std::cout << "Headers:" << std::endl;
    for (const auto& [key, value] : response.headers) {
        std::cout << "  " << key << ": " << value << std::endl;
    }
    std::cout << "Body length: " << response.text.size() << " bytes" << std::endl;
    std::cout << "Body: " << response.text << std::endl;
}

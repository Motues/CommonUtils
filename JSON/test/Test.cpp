#include "Utils/JSON.hpp"
#include <iostream>

int main() {
    using namespace Utils::JSON;

    JsonManager json = JsonManager::FromFile("test.json");

    std::cout << json["name"].String() << std::endl;
    std::cout << json["version"].String() << std::endl;
    std::cout << json["array"][0].Int()  << std::endl;
    std::cout << json["object"]["a"].Int()  << std::endl;

    json["array"][0].set(20);
    json.saveToFile("test_new.json");

    return 0;
}
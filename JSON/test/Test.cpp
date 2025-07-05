#include "Utils/JSON.hpp"
#include <iostream>

int main() {
    using namespace Utils::JSON;

    JsonManager json = JsonManager::FromFile("test.json");

    std::cout << json["name"].String() << std::endl;
    std::cout << json["version"].String() << std::endl;
    std::cout << json["array"][0].Int() << std::endl;
    std::cout << json["object"]["a"].Int() << std::endl;


    json["time"] = "2025-01-01";
    json["array"][0] = 20;
    json["object"]["d"] = 5;
    json.saveToFile("test_new.json");

    std::cout << json.toString(4) << std::endl;

    return 0;
}
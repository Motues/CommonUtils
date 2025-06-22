#include <iostream>
#include <string>
#include "Utils/TCP.hpp"

void RunClient() {

    using namespace Utils::TCP;

    TCPClient client("127.0.0.1", 3030, "$$");
    if (!client.ConnectToServer()) {
        std::cerr << "Failed to connect to server." << std::endl;
        return;
    }

    while (true) {
        // 从控制台读取用户输入
        std::string message;
        std::cout << "Enter message to send: ";
        std::cin >> message;

        if (message.empty()) {
            std::cout << "Empty message, skipping..." << std::endl;
            continue;
        }

        // 发送消息到服务器
        if (!client.SendData(message + "$$")) {
            std::cerr << "Failed to send data to server." << std::endl;
            continue;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // 接收服务器响应
        std::string response;
        if (client.RecData(response)) {
            std::cout << "Server response: " << response << std::endl;
        }
    }
}

int main() {
    RunClient();
    return 0;
}
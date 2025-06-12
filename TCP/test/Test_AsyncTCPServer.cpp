#include <iostream>
#include "Utils/TCP.hpp"
#include "Utils/MessageQueue.hpp"

using namespace Utils::MessageQueue;
using namespace Utils::TCP;


int main() {
    Utils::TCP::AsyncTCPServer server(1717, "127.0.0.1", IPType::IPV4);
    if (!server.StartServer()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    while (true) {
        TCPMessage msg;
        if (server.GetMessage(msg)) {
            std::cout << "Received from: " << msg.clientPtr->address << ":" << msg.clientPtr->port << std::endl;
            std::cout << "Received message: " << msg.data << std::endl;
            server.SendData("Hello from server", msg.clientPtr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // 主线程可以继续执行其他任务
    // ...

    server.CloseServer();
    return 0;
}

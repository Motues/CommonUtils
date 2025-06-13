#include <iostream>
#include <fstream>
#include "Utils/TCP.hpp"
#include "Utils/MessageQueue.hpp"

using namespace Utils::MessageQueue;
using namespace Utils::TCP;


int main() {
    Utils::TCP::AsyncTCPServer server(3030, "0.0.0.0", IPType::IPV4);
    if (!server.StartServer()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    auto file = std::ofstream("log.txt", std::ios::out | std::ios::app);

    while (true) {
        TCPMessage msg;
        if (server.GetMessage(msg)) {
            std::cout << "Received from: " << msg.clientPtr->address << ":" << msg.clientPtr->port << std::endl;
            std::cout << "Received message: " << msg.data << std::endl;
            if(msg.data == "exit"){
                server.PostCloseClient(msg.clientPtr);
                std::cout  << "Closing client" << std::endl;
                continue;
            }
            // 时间格式化
            std::time_t now_c = std::chrono::system_clock::to_time_t(msg.time);
            std::tm tm;
#ifdef _WIN32
            localtime_s(&tm, &now_c);
#else
            localtime_r(&now_c, &tm);
#endif
            char time_str[20];  // 足够容纳 "YYYY-MM-DD HH:MM:SS"
            std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm);
            file << "Time: " << time_str << std::endl;
            file << "Received from: " << msg.clientPtr->address << ":" << msg.clientPtr->port << std::endl;
            file << "Received message: " << msg.data << std::endl <<  std::endl;

            server.SendData("Hello from server", msg.clientPtr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // 主线程可以继续执行其他任务
    // ...

    server.CloseServer();
    return 0;
}

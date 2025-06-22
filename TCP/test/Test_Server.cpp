#include "Utils/TCP/Server.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// 回调函数：模拟消息处理逻辑
void OnMessageReceived(const Utils::TCP::TCPMessage& message) {
    std::cout << "Received Message from Session ID: " << message.sessionId
              << " (" << message.ip << ":" << message.port << ")"
              << " at " << std::chrono::duration_cast<std::chrono::seconds>(
        message.time.time_since_epoch()).count()
              << "s: " << message.data << std::endl;
}

int main() {

        // 创建服务器实例（默认端口3030，最大会话数64）
        Utils::TCP::TCPServer server(3030, "$$");

        // 启动服务器
        server.Start();
        std::cout << "Server started on port 3030..." << std::endl;

        // 模拟接收消息的循环
            while (true) {
                Utils::TCP::TCPMessage message;
                server.GetMessage(message);
                if (!message.data.empty()) {
                    OnMessageReceived(message);
                }
            }

    return 0;
}

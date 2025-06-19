#include "Utils/TCP/Session.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>

// 简单的消息处理函数，用于测试
void OnMessageReceived(Utils::TCP::SessionPtr session, const std::string& message) {
    std::cout << "Received message from session " << session->GetSessionId()
              << " [" << session->GetAddress() << ":" << session->GetPort() << "] : "
              << message << std::endl;
}

int main() {
    boost::asio::io_context ioContext;

    // 创建一个SessionManager实例，超时时间为5秒
    Utils::TCP::SessionManager sessionManager(std::chrono::seconds(5));

    // 创建两个会话
    auto session1 = std::make_shared<Utils::TCP::Session>(ioContext);
    auto session2 = std::make_shared<Utils::TCP::Session>(ioContext);

    // 为会话设置消息处理函数
    session1->SetMessageHandler(OnMessageReceived);
    session2->SetMessageHandler(OnMessageReceived);

    // 添加会话到管理器
    int sessionId1 = sessionManager.AddSession(session1);
    int sessionId2 = sessionManager.AddSession(session2);

    std::cout << "Current session count: " << sessionManager.SessionCount() << std::endl;

    // 启动会话
    session1->Start();
    session2->Start();

    // 给会话一些时间来启动
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 发送单播消息
    std::cout << "Sending unicast message to session " << sessionId1 << std::endl;
    sessionManager.Unicast(sessionId1, "Hello, Session 1!\n");

    // 给会话一些时间来处理消息
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 发送广播消息
    std::cout << "Sending broadcast message" << std::endl;
    sessionManager.Broadcast("Hello, All Sessions!\n");

    // 给会话一些时间来处理消息
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 检查会话超时
    std::cout << "Waiting for sessions to timeout (5 seconds)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(6));

    sessionManager.CleanExpiredSession();
    std::cout << "Current session count after cleanup: " << sessionManager.SessionCount() << std::endl;

    // 移除一个会话
    std::cout << "Removing session " << sessionId2 << std::endl;
    sessionManager.RemoveSession(sessionId2);

    std::cout << "Final session count: " << sessionManager.SessionCount() << std::endl;

    // 运行IO上下文
    ioContext.run();

    return 0;
}

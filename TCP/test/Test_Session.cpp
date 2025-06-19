#include "Utils/TCP/Session.hpp"
#include <iostream>

using namespace Utils::TCP;

void OnMessage(SessionPtr session, const std::string& data) {
    std::cout << "Session received: " << data << std::endl;
}

int main() {
    // ===== 启动客户端：连接到远程服务器 =====
    IOContext ioContext;
    TCPEndPoint endpoint(boost::asio::ip::make_address("127.0.0.1"), 3030);
    TCPSocket socket(ioContext);
    socket.connect(endpoint);

    // ✅ 改为 shared_ptr 管理 Session 生命周期
    SessionPtr session = std::make_shared<Session>(socket);
    session->SetMessageHandler(OnMessage);
    std::cout << "Client connected to remote server" << std::endl;
    // 启动异步读取
    session->Start();
    std::cout << "Client started" << std::endl;

    std::thread inputThread([&session]() {
        while (true) {
            std::string data;
            getline(std::cin, data);
            if (data == "exit") {
                session->Close();
                break;
            }
            session->SentData(data + "\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    ioContext.run(); // 主线程运行 io_context

    inputThread.join(); // 可选等待输入线程结束std::cout << "Session sent: " << data << std::endl;

    return 0;
}

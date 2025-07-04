#include <csignal>
#include <iostream>
#include "Utils/Network/HTTPS.hpp"

std::atomic<bool> stopSignal(false);

using namespace Utils::Network;

void Hello_World(HTTPRequestString& req, HTTPResponseString& res) {
    res.set(HTTP::field::server, "C++/Network");
    res.set(HTTP::field::content_type, "text/plain");
    res.body() = "Hello World!";
    res.prepare_payload();
}

void SignalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        stopSignal = true;
    }
}
int main() {
    HTTPS::HTTPServer server(1717);
    server.AddRoute("/hello", "GET", Hello_World);
    server.Start();

    std::signal(SIGINT, SignalHandler);     // Ctrl + C
    std::signal(SIGTERM, SignalHandler);   // kill 命令等

    std::cout << "Server is running... Press Ctrl+C to exit." << std::endl;

    while (!stopSignal) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Shutting down server..." << std::endl;
    server.Stop();

    return 0;
}
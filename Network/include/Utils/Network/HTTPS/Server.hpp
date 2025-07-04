#pragma once

#include "Session.hpp"
#include "Router.hpp"

#include <functional>

namespace Utils :: Network {


namespace HTTPS {

    class HTTPServer {
    public:
        HTTPServer(int port);

        void AddRoute(const std::string& path, const std::string& method, HTTPRequestHandler handler);
        void Start();
        void Stop();

    private:
        using WorkGuard = boost::asio::executor_work_guard<IOContext::executor_type>;
        using WorkGuardPtr = std::shared_ptr<WorkGuard>;
        int port_;
        IOContextPtr ioContextPtr_;
        TCPAcceptorPtr acceptorPtr_;
        WorkGuardPtr workGuardPtr_;
        std::thread ioThread_;
        Router router_;
        std::atomic<bool> isRunning_;

        void DoAccept();

    };

}
}
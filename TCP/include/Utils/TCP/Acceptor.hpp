#pragma once

#include <functional>
#include <memory>

#include "BasicType.hpp"

namespace Utils :: TCP {

    class IOContextPool {
    public:
        explicit IOContextPool(size_t poolSize = (std::thread::hardware_concurrency() + 1) / 2);
        ~IOContextPool();

        void Start();
        void Stop();
        IOContext& GetIOContext();

    private:

        using WorkGuard = boost::asio::executor_work_guard<IOContext::executor_type>;

        std::vector<IOContextPtr>  ioContextPtrs_;
        std::vector<WorkGuard>  workGuards_; // 保持引用，避免 ioContextPtr销毁
        std::vector<std::thread>  threads_;
        std::atomic<size_t>   nextIOContextIndex_;
    };

    using ConnectionHandler = std::function<void(TCPSocketPtr)>;

    class Acceptor {
    public:
        Acceptor(IOContextPool& ioContextPool, const TCPEndPoint& endPoint);
        ~Acceptor();

        void Start(ConnectionHandler connectionHandler);
        void Stop();

    private:
        IOContextPool& ioContextPool_; // 引用传入，便于多个 Acceptor 共享一个 IOContextPool
        TCPAcceptor acceptor_;
        ConnectionHandler connectionHandler_;
        bool isStarted_ = false;

        void DoAccept();
    };

    using IOContextPoolPtr = std::shared_ptr<IOContextPool>;
    using AcceptorPtr = std::shared_ptr<Acceptor>;


}

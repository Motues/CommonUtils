#include <iostream>
#include "Utils/TCP/Acceptor.hpp"

namespace Utils :: TCP {

    IOContextPool::IOContextPool(size_t poolSize) {
        if (poolSize == 0) poolSize = (std::thread::hardware_concurrency() + 1) / 2;
        ioContextPtrs_.reserve(poolSize);
        workGuards_.reserve(poolSize);
        for (size_t i = 0; i < poolSize; ++i) {
            auto io_context_ptr = std::make_shared<IOContext>();
            ioContextPtrs_.emplace_back(io_context_ptr);
            workGuards_.emplace_back(io_context_ptr->get_executor());
        }
    }
    IOContextPool::~IOContextPool() {
        Stop();
    }

    IOContext& IOContextPool::GetIOContext() {
        size_t index = nextIOContextIndex_.fetch_add(1, std::memory_order_relaxed);
        return *ioContextPtrs_[index % ioContextPtrs_.size()];
    }

    void IOContextPool::Start() {
        for(size_t i = 0; i < ioContextPtrs_.size(); ++i) {
            threads_.emplace_back([this, i]() {
                ioContextPtrs_[i]->run();
            });
        }
    }

    void IOContextPool::Stop() {
        workGuards_.clear();
        for(auto& io_context_ptr : ioContextPtrs_) {
            io_context_ptr->stop();
        }
        for(auto& thread : threads_) {
            if(thread.joinable()) {
                thread.join();
            }
        }
        threads_.clear();
        ioContextPtrs_.clear();
    }



    Acceptor::Acceptor(IOContextPool& ioContextPool, const TCPEndPoint &endPoint) :
        ioContextPool_(ioContextPool), acceptor_(ioContextPool.GetIOContext()) {
        BoostErrorCode ec;
        acceptor_.open(endPoint.protocol(), ec);
        if(ec) throw std::runtime_error("Failed to open acceptor" + ec.message());
        acceptor_.set_option(boost::asio::socket_base::reuse_address(true),  ec);
        if(ec) throw std::runtime_error("Failed to set option" + ec.message());
        acceptor_.bind(endPoint, ec);
        if(ec) throw std::runtime_error("Failed to bind" + ec.message());
        acceptor_.listen(boost::asio::socket_base::max_connections, ec);
        if(ec) throw std::runtime_error("Failed to listen" + ec.message());
    }

    Acceptor::~Acceptor() {
        acceptor_.close();
    }

    void Acceptor::Start(Utils::TCP::ConnectionHandler connectionHandler) {
        connectionHandler_ = std::move(connectionHandler);
        isStarted_ = true;
        DoAccept();
    }

    void Acceptor::DoAccept() {
        if(!isStarted_) return;
        auto socketPtr = std::make_shared<TCPSocket>(ioContextPool_.GetIOContext());
        acceptor_.async_accept(*socketPtr, [this, socketPtr](const BoostErrorCode &ec) {
            if(!ec) {
                if(connectionHandler_) {
                    connectionHandler_(socketPtr);
                }
            }else {
                std::cout << "Failed to accept" << ec.message() << std::endl;
            }
            DoAccept();
        });
    }

    void Acceptor::Stop() {
        isStarted_ = false;
        BoostErrorCode ec;
        acceptor_.close(ec);
    }
}
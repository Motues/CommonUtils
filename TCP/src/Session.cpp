#include "Utils/TCP/Session.hpp"

namespace Utils :: TCP {

    Session::Session(IOContext& ioContext, Seconds clockTime) :
        socket_(ioContext), lastActiveTime_(SystemClock::now()), clockTime_(clockTime) {}

    Session::Session(Utils::TCP::TCPSocket &socket, Utils::TCP::Seconds clockTime) :
        socket_(std::move(socket)), lastActiveTime_(SystemClock::now()), clockTime_(clockTime){}

    Session::~Session() {
        Close();
    }

    TCPSocket& Session::GetSocket() {
        return socket_;
    }
    int& Session::GetSessionId() {
        return sessionId_;
    }
    int Session::GetPort() {
        return socket_.local_endpoint().port();
    }
    std::string Session::GetAddress() {
        return socket_.remote_endpoint().address().to_string();
    }


    void Session::Start() {
        DoRead();
    }

    void Session::DoRead() {
        auto self = shared_from_this();
        boost::asio::async_read_until(socket_, readBuffer_, '\n',
                                      [this, self](const BoostErrorCode& ec, std::size_t bytes_transferred) {
                                          AsyncRead(ec, bytes_transferred);
                                      });
    }

    void Session::AsyncRead(const BoostErrorCode& ec, std::size_t bytes_transferred) {
        if (!ec) {
            std::istream input(&readBuffer_);
            std::string data;
            std::getline(input, data);
            if(messageHandler_) {
                messageHandler_(shared_from_this(), data);
            }
            lastActiveTime_ = SystemClock::now();
            DoRead();
        } else {
            Close();
        }
    }

    void Session::SentData(const std::string &data) {
        std::lock_guard<std::mutex> lock(writeMutex_);
        bool writeInProgress = !writeQueue_.empty();
        writeQueue_.push_back(data);
        if(!writeInProgress) {
            DoWrite();
        }
    }

    void Session::DoWrite() {
        auto self = shared_from_this();
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(writeQueue_.front()),
                                 [this, self](const BoostErrorCode & ec, std::size_t bytes_transferred) {
                                     AsyncWrite(ec, bytes_transferred);
                                 });
    }

    void Session::AsyncWrite(const BoostErrorCode &ec, std::size_t bytes_transferred) {
        std::lock_guard<std::mutex> lock(writeMutex_);
        if (!ec) {
            writeQueue_.pop_front();
            if (!writeQueue_.empty()) {
                DoWrite();
                lastActiveTime_ = SystemClock::now();
            }
        } else {
            Close();
        }
    }

    void Session::Close() {
        BoostErrorCode ec;
        socket_.shutdown(TCPSocket::shutdown_both, ec);
        socket_.close(ec);
    }

    bool Session::CheckTimeout() {
        auto  now = SystemClock::now();
        return now - lastActiveTime_ > clockTime_;
    }

    bool Session::CheckTimeout(Seconds clockTime) {
        auto  now = SystemClock::now();
        return now - lastActiveTime_ > clockTime;
    }

    void Session::SetMessageHandler(MessageHandler messageHandler) {
        messageHandler_ = messageHandler;
    }


    SessionManager::SessionManager(Seconds clockTime) :
        clockTime_(clockTime), sessionId_(1) {}

    SessionManager::~SessionManager() {
    }

    int SessionManager::AddSession(Utils::TCP::SessionPtr sessionPtr) {
        int sessionId = sessionId_.fetch_add(1);
        {
            std::unique_lock lock(sessionMutex_);
            // TODO 根据客户端生成唯一的uid
            sessionPtr->GetSessionId() = sessionId;
            sessions_[sessionId] = std::move(sessionPtr);
        }
        return sessionId;
    }

    void SessionManager::RemoveSession(int sessionId) {
        std::unique_lock lock(sessionMutex_);
        auto it = sessions_.find(sessionId);
        if (it != sessions_.end()) {
            it->second->Close();
            sessions_.erase(it);
        }
    }

    void SessionManager::Unicast(int sessionId, const std::string& data) {
        std::unique_lock lock(sessionMutex_);
        auto it = sessions_.find(sessionId);
        if (it != sessions_.end()) {
            it->second->SentData(data);
        }
    }

    void SessionManager::Broadcast(const std::string& data) {
        std::unique_lock lock(sessionMutex_);
        for (auto& session : sessions_) {
            session.second->SentData(data);
        }
    }

    void SessionManager::CleanExpiredSession() {
        std::vector<int> expiredSessionIds;
        {
            std::unique_lock lock(sessionMutex_);
            for (const auto& [id, session] : sessions_) {
                // TODO 添加超时检测选择模式
                if (session && session->CheckTimeout(clockTime_)) {
                    expiredSessionIds.push_back(id);
                }
            }
        }
        for (auto sessionId : expiredSessionIds) {
            RemoveSession(sessionId);
        }
    }

    int SessionManager::SessionCount() const {
        std::unique_lock lock(sessionMutex_);
        return sessions_.size();
    }

}
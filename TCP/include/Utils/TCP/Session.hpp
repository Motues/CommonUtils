#pragma once

#include <queue>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <memory>

#include "BasicType.hpp"
#include "Utils/MessageQueue.hpp"


namespace Utils :: TCP {




    class Session : public std::enable_shared_from_this<Session> {
    public:

        using SessionPtr = std::shared_ptr<Session>;
        using MessageHandler = std::function<void(SessionPtr, const std::string&)>;

        Session(IOContext &ioContext, std::string endOfMessage = "\n", Seconds clockTime = Seconds(30));
        Session(TCPSocket &socket, std::string endOfMessage = "\n", Seconds clockTime = Seconds(30));
        ~Session();

        TCPSocket& GetSocket();                      // 获取socket
        int& GetSessionId();
        int GetPort();
        std::string GetAddress();

        void Start();                                // 启动异步读取数据
        void SentData(const  std::string& data);     // 发送数据
        void Close();                                // 关闭连接
        bool CheckTimeout();                         // 检查连接是否超时，内部使用
        bool CheckTimeout(Seconds clockTime);        // 检查连接是否超时，给外部使用

        void SetMessageHandler(MessageHandler messageHandler); // 设置消息处理函数

    private:
        TCPSocket socket_;
        int sessionId_;
        std::string endOfMessage_;
        BoostStreamBuffer readBuffer_;
        std::deque<std::string> writeQueue_;
        std::mutex writeMutex_;
        MessageHandler messageHandler_; // 通过回调函数处理消息，可以传入自定义处理函数

        TimePoint lastActiveTime_;                   // 上次活动时间
        Seconds clockTime_{30};

        void DoRead(); // 读取数据
        void DoWrite(); // 发送数据
        void AsyncRead(const BoostErrorCode& errorCode, std::size_t bytesTransferred);  // 异步读取数据
        void AsyncWrite(const BoostErrorCode& errorCode, std::size_t bytesTransferred); // 异步发送数据
    };

    using SessionPtr = Session::SessionPtr;
    using MessageHandler = Session::MessageHandler;

    class SessionManager {
    public:
        SessionManager(Seconds clockTime = Seconds(30));
        ~SessionManager();

        int AddSession(SessionPtr sessionPtr);
        void RemoveSession(int sessionId);
        void CleanExpiredSession();

        void Unicast(int sessionId, const std::string& data);
        void Broadcast(const std::string& data);
        int SessionCount() const;

    private:
        mutable std::shared_mutex sessionMutex_;
        std::unordered_map<int, SessionPtr> sessions_;
        std::atomic<int> sessionId_;
        Seconds  clockTime_{30};            // 检测超时时间，如果是 0 则不检测
    };

    using SessionManagerPtr = std::shared_ptr<SessionManager>;

}
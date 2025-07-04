#pragma once

#include "../BasicType.hpp"

namespace Utils :: Network {
namespace WebSocket {

    struct WebSocketMessage {
        int sessionId;
        std::string message;
        SystemTime time;
    };
    using WebSocketMessageQueue = MessageQueue<WebSocketMessage>;

}
}
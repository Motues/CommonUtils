#pragma once

#include "../BasicType.hpp"
#include "Router.hpp"

namespace Utils :: Network {

namespace HTTPS {

    class Session : public std::enable_shared_from_this<Session>{
    public:
        Session(TCPSocket&& socket);
        Session(IOContext& ioContext);
        void SetRouter(Router& router);
        void Start();
        void Close();

    private:
        TCPSocket socket_;
        Router* routerPtr_;
        Beast::flat_buffer buffer_;
        HTTPRequestString request_;

        void DoRead();
        void RequestHandler();
    };

}
}
#ifndef P2PD_WEBSOCKET_SERVER_H
#define P2PD_WEBSOCKET_SERVER_H

#include <condition_variable>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "base_server.h"
#include "engine/observer.h"
#include "websocket/session.h"
#include "websocket/session_host.h"

namespace p2pd {
namespace websocket {

// namespace shortcut
namespace asio  = boost::asio;
// typename shortcut
using error_code = boost::system::error_code;


class Server :  public p2pd::BaseServer, 
                public p2pd::websocket::SessionHost,
                public p2pd::engine::Observer {

using endpoint_type = asio::ip::tcp::endpoint;
using acceptor_type = asio::ip::tcp::acceptor;
using socket_type   = asio::ip::tcp::socket;

private:
    acceptor_type acceptor_;
    std::map<std::string, std::shared_ptr<Session>> sessions_{};
    std::condition_variable cond_{};

public:
    explicit Server(
        std::shared_ptr<p2pd::engine::Engine> engine, 
        boost::asio::io_context & io_ctx);

    // Override |p2pd::BasicServer|
    void Startup(uint16_t port) final;
    void Shutdown() final;
    // Override |p2pd::websocket::SessionHost|
    void OnSessionMessage(
            std::shared_ptr<Session> session, std::string message) final;
    void OnSessionClose(
            std::shared_ptr<Session> session) final;
    // Override |p2pd::engine::Observer|
    void OnTaskAdd() final;
    void OnTaskStateChanged(uint32_t task_id) final;

private:
    void DoAccept();
    void OnAccepted(error_code const& ec, socket_type socket);
};

} // namespace websocket
} // namespace p2pd

#endif // P2PD_WEBSOCKET_SERVER_H

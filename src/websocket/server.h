#ifndef P2PD_WEBSOCKET_SERVER_H
#define P2PD_WEBSOCKET_SERVER_H

#include <condition_variable>
#include <cstdint>
#include <map>
#include <memory>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "engine/engine.h"
#include "engine/observer.h"
#include "websocket/session.h"
#include "websocket/session_host.h"

namespace p2pd {
namespace websocket {

// namespace shortcut
namespace asio  = boost::asio;
// typename shortcut
using error_code = boost::system::error_code;
using io_context = asio::io_context;

/**
 * @brief Websocket API Server.
 * 
 * @author Tomo Kunagisa
 */
class Server :  public p2pd::websocket::SessionHost,
                public p2pd::engine::Observer {

using endpoint_type = asio::ip::tcp::endpoint;
using acceptor_type = asio::ip::tcp::acceptor;
using socket_type   = asio::ip::tcp::socket;

private:
    // P2P engine.
    std::shared_ptr<engine::Engine> engine_;
    // IO context for network.
    io_context & io_ctx_;
    // Incoming connection acceptor.
    acceptor_type acceptor_;
    // Store all alive sessions.
    std::map<std::string, std::unique_ptr<Session>> sessions_{};
    // Waiter for shutdown job.
    std::condition_variable cond_{};

public:
    // Constructor
    explicit Server(
        std::shared_ptr<p2pd::engine::Engine> engine, 
        io_context & io_ctx
    );
    // Destructor
    ~Server();

    // Public API
    void Startup(uint16_t port);
    void Shutdown();

    // Override |p2pd::websocket::SessionHost|
    void OnSessionMessage(
            Session * session, std::string message) final;
    void OnSessionClose(
            Session * session) final;
    // Override |p2pd::engine::Observer|
    void OnAlert(std::string const& alert_message) final;
    void OnTaskAdd() final;
    void OnTaskStateChanged(uint32_t task_id) final;

private:
    void DoAccept();
    void OnAccepted(error_code const& ec, socket_type socket);
};

std::unique_ptr<Server> create_server(
    std::shared_ptr<p2pd::engine::Engine> engine, 
    io_context & io_ctx
);

} // namespace websocket
} // namespace p2pd

#endif // P2PD_WEBSOCKET_SERVER_H

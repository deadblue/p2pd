#ifndef P2PD_API_SERVER_H
#define P2PD_API_SERVER_H

#include <condition_variable>
#include <cstdint>
#include <map>
#include <memory>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "api/controller.h"
#include "api/server_session.h"
#include "api/server_session_host.h"
#include "engine/engine.h"

namespace p2pd {
namespace api {

// namespace shortcut
namespace asio  = boost::asio;
// typename shortcut
using error_code = boost::system::error_code;
using io_context = asio::io_context;

/**
 * @brief Server only does the underlying message transmission.
 * 
 * @author deadblue
 */
class Server :  public ServerSessionHost {

private:
    using acceptor_type = asio::ip::tcp::acceptor;
    using endpoint_type = asio::ip::tcp::endpoint;
    using socket_type   = asio::ip::tcp::socket;

    using controller_ptr = std::unique_ptr<Controller>;
    using engine_ptr     = std::shared_ptr<engine::Engine>;
    using session_ptr    = std::unique_ptr<ServerSession>;

    // API Controller
    controller_ptr ctrl_{};
    // IO context for network.
    io_context & io_ctx_;
    // Incoming connection acceptor.
    acceptor_type acceptor_;
    // Store all alive sessions.
    std::map<session_id, session_ptr> sessions_{};
    // Waiter for shutdown job.
    std::condition_variable cond_{};

public:
    // Constructor
    explicit Server(
        engine_ptr engine, io_context & io_ctx
    );
    // Destructor
    ~Server();

    // Public API
    void Startup(const char * ip, uint16_t port);
    void Shutdown();

    // Override |p2pd::api::ServerSessionHost|
    void OnSessionMessage(
            session_id id, std::string message) final;
    void OnSessionClose(
            session_id id) final;

private:
    void DoAccept();
    void OnAccepted(error_code const& ec, socket_type socket);
    void SendMessageTo(session_id id, std::string message);
    void PublishMessage(std::string message);

};

std::unique_ptr<Server> create_server(
    std::shared_ptr<p2pd::engine::Engine> engine, 
    io_context & io_ctx
);

} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVER_H

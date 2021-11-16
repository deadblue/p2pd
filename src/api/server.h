#ifndef P2PD_API_SERVER_H
#define P2PD_API_SERVER_H

#include <condition_variable>
#include <cstdint>
#include <map>
#include <memory>

#include "api/server/session_host.h"
#include "api/server/type.h"

namespace p2pd {

namespace engine {
class Engine;
} // namespace engine

namespace api {

class Controller;
class Session;

using engine_ptr = std::shared_ptr<engine::Engine>;

/**
 * @brief Server only does the underlying message transmission.
 * 
 * @author deadblue
 */
class Server :  public SessionHost {

private:
    using controller_ptr = std::unique_ptr<Controller>;
    using session_ptr    = std::shared_ptr<Session>;

    // Executor for network operations.
    io_context io_ctx_;
    // Incoming connection acceptor.
    acceptor acceptor_;
    // Store all alive sessions.
    std::map<session_id, session_ptr> sessions_{};
    // Waiter for shutdown job.
    std::condition_variable cond_{};
    // API Controller
    controller_ptr ctrl_{};

public:
    // Constructor
    explicit Server(engine_ptr engine);
    // Destructor
    ~Server();

    // Public API
    void Startup(const char * ip, uint16_t port);
    void Shutdown();

    // Override |p2pd::api::server::SessionHost|
    void OnSessionMessage(
            session_id id, std::string message) final;
    void OnSessionClose(
            session_id id) final;

private:
    void DoAccept();
    void OnAccepted(error_code const& ec, socket s);
    /**
     * Push message to a session.
     * @param id       Session ID.
     * @param message  Message to push.
     */
    void PushMessage(session_id id, std::string message);
    /**
     * Broadcast message to all sessions.
     * @param message  Message to broadcast.
     */
    void BroadcastMessage(std::string message);

};

std::unique_ptr<Server> create_server(engine_ptr engine);

} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVER_H

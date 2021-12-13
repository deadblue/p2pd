#ifndef P2PD_API_SERVER_SESSION_H
#define P2PD_API_SERVER_SESSION_H

#include <atomic>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include "api/server/session_host.h"
#include "api/server/type.h"

namespace p2pd {
namespace api {

/**
 * @brief ServerSession holds a connection which initiated from client.
 * 
 * @author deadblue
 */
class Session : public std::enable_shared_from_this<Session> {

private:
    // Session ID.
    session_id id_;
    // Websocket stream.
    stream stream_;
    // Pointer to session host.
    SessionHost * host_;
    // Executor for reading operations.
    strand r_strand_;
    // Buffer for reading.
    buffer r_buf_{};
    // Executor for writing operations.
    strand w_strand_;
    // Close flag.
    std::atomic_bool closed_{false};

public:
    // Constructor
    Session(io_context & io_ctx, socket s, SessionHost * host);
    // Destructor
    ~Session();

    // Retrieve session id.
    session_id const& id() const { return id_; }
    /**
     * @brief Start handshake with client.
     */
    void Open();
    /**
     * @brief Close session from server side.
     */
    void Close();
    /**
     * @brief Asynchronously send message to client.
     * @param message Message to send.
     */
    void SendMessage(std::string message);

private:
    // Callback function when handshake is done.
    void OnOpened(error_code const& ec);
    // Callback function when session closed by local/remote.
    void OnClosed(error_code const& ec);
    // Read message.
    void AsyncRead();
    // Callback function when receive data from remote.
    void OnRead(error_code const& ec, size_t transferred_size);
    // Write text data to remote.
    void DoWrite(std::string message);

};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVER_SESSION_H

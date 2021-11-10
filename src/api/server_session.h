#ifndef P2PD_API_SERVER_SESSION_H
#define P2PD_API_SERVER_SESSION_H

#include <atomic>
#include <queue>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include "api/server_session_host.h"

namespace p2pd {
namespace api {

// namespace shortcut
namespace asio = boost::asio;
namespace ws = boost::beast::websocket;

/**
 * @brief ServerSession holds a connection which initiated from client.
 * 
 * @author deadblue
 */
class ServerSession {

private:
    using error_code  = boost::system::error_code;
    using io_context  = asio::io_context;
    using strand_type = asio::io_context::strand;
    using socket_type = asio::ip::tcp::socket;
    using stream_type = ws::stream<socket_type>;
    using buffer_type = boost::beast::multi_buffer;

    // Session ID.
    session_id id_;
    // Websocket stream
    stream_type stream_;
    // Pointer to session host.
    ServerSessionHost * host_;
    // Executor for reading operations
    strand_type r_strand_;
    // Executor for writing operations
    strand_type w_strand_;
    // Buffer for reading.
    buffer_type r_buf_{};
    // Buffer for writing
    buffer_type w_buf_{};

public:
    // Constructor
    ServerSession(socket_type sock, ServerSessionHost * host, io_context & io_ctx);
    // Destructor
    ~ServerSession();

    // Retrieve session id.
    session_id const& id() const { return id_; }
    // Open the session.
    void Open();
    // Close the session, send close message to client.
    void Close();
    // Send message to client.
    void SendMessage(std::string const& message);

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
    void DoWrite(size_t data_size);

};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVER_SESSION_H

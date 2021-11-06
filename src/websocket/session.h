#ifndef P2PD_WEBSOCKET_SESSION_H
#define P2PD_WEBSOCKET_SESSION_H

#include <atomic>
#include <memory>
#include <queue>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include "websocket/session_host.h"

namespace p2pd {
namespace websocket {

// namespace shortcut
namespace asio = boost::asio;
namespace ws = boost::beast::websocket;
// type shortcut
using error_code    = boost::system::error_code;
using io_context    = asio::io_context;
using strand_type   = asio::io_context::strand;
using socket_type   = asio::ip::tcp::socket;
using endpoint_type = asio::ip::tcp::endpoint;
using stream_type   = ws::stream<socket_type>;
using buffer_type   = boost::beast::multi_buffer;

/**
 * @brief Session holds a connection from client.
 * 
 * @author deadblue
 */
class Session : public std::enable_shared_from_this<Session> {

private:

    // Session ID.
    std::string id_;
    
    // Websocket stream
    stream_type stream_;

    // Pointer to session host.
    SessionHost * host_;

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
    Session(socket_type sock, SessionHost * host, io_context & io_ctx);
    // Destructor
    ~Session();
    // Retrieve session id.
    std::string const& id() const { return id_; }
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

} // namespace websocket
} // namespace p2pd

#endif // P2PD_WEBSOCKET_SESSION_H

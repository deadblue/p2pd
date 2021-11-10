#include "api/server_session.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "common/logging.h"

namespace p2pd {
namespace api {

// namespace shortcut
namespace chrono = std::chrono;
// type shortcut
using websocket_error = boost::beast::websocket::error;

ServerSession::ServerSession(
    socket_type sock, ServerSessionHost * host, io_context & io_ctx
) : stream_(std::move(sock)), 
    host_(host), 
    r_strand_(io_ctx), 
    w_strand_(io_ctx) {
    id_ = reinterpret_cast<uintptr_t>(this);
}

ServerSession::~ServerSession() {
    LOG << "Session exit, id=" << id_;
}

void ServerSession::Open() {
    // Perform server-side handshake
    stream_.async_accept(asio::bind_executor(r_strand_, std::bind(
        &ServerSession::OnOpened, this, std::placeholders::_1
    )));
}

void ServerSession::Close() {
    // Send close message
    auto reason = ws::close_reason(ws::close_code::normal);
    stream_.async_close(reason, asio::bind_executor(w_strand_, std::bind(
        &ServerSession::OnClosed, this, std::placeholders::_1
    )));
}

void ServerSession::SendMessage(std::string const& message) {
    if(message.empty()) { return; }
    // Put message to write buffer
    auto * data = message.c_str();
    auto data_size = message.size();
    auto bufs = w_buf_.prepare(data_size);
    for(auto const& buf : bufs) {
        memcpy(buf.data(), data, buf.size());
        data += buf.size();
    }
    // Enqueueu writing task
    asio::post(w_strand_, std::bind(&ServerSession::DoWrite, 
        this, data_size
    ));
}

void ServerSession::OnOpened(error_code const& ec) {
    AsyncRead();
}

void ServerSession::OnClosed(error_code const& ec) {
    if(!ec  // Successfully closed from server.
        && ec != websocket_error::closed    // Gracefully closed from client.
        && ec != asio::error::operation_aborted // Abort connection from client (Linux).
        && ec != asio::error::not_connected     // Abort connection from client (macOS).
    ) {
        LOG << "Close session on error(" << ec.value()
            << "): " << ec.message();
    }
    host_->OnSessionClose(id_);
}

void ServerSession::AsyncRead() {
    stream_.async_read(r_buf_, asio::bind_executor(r_strand_, std::bind(
        &ServerSession::OnRead, this,
        std::placeholders::_1, std::placeholders::_2
    )));
}

void ServerSession::OnRead(error_code const& ec, size_t transferred_size) {
    if(ec) {
        OnClosed(ec);
        return;
    }
    // Handle incoming message
    if(stream_.got_text()) {
        auto message = boost::beast::buffers_to_string(r_buf_.data());
        host_->OnSessionMessage(id_, message);
    }
    r_buf_.consume(transferred_size);
    // Read next message
    AsyncRead();
}

void ServerSession::DoWrite(size_t data_size) {
    // Moving data from output buffer to input buffer
    w_buf_.commit(data_size);
    // Send text data
    stream_.text(true);
    error_code ec;
    auto written_size = stream_.write(w_buf_.data(), ec);
    // Error handling
    if(ec) {
        OnClosed(ec);
    } else {
        if(written_size != data_size) {
            LOG << "Incomplete writting: " 
                << written_size << "!=" << data_size;
        }
        w_buf_.consume(written_size);
    }
}

} // namespace api
} // namespace p2pd
#include "websocket/session.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "common/logging.h"

namespace p2pd {
namespace websocket {

// namespace shortcut
namespace chrono = std::chrono;
// type shortcut
using websocket_error = boost::beast::websocket::error;

std::string generate_id(endpoint_type const& endpoint) {
    auto now = chrono::duration_cast<chrono::seconds>(
        chrono::system_clock::now().time_since_epoch()
    ).count();
    auto addr = endpoint.address().to_v4().to_bytes();

    char buf[32];
    snprintf(buf, 32, "%016x_%02x%02x%02x%02x_%04x", 
        now, addr[0], addr[1], addr[2], addr[3], endpoint.port()
    );
    return std::string{buf};
}

Session::Session(
    socket_type sock, SessionHost * host, io_context & io_ctx
) : id_(generate_id(sock.remote_endpoint())),
    stream_(std::move(sock)), host_(host),
    r_strand_(io_ctx), w_strand_(io_ctx) {}

Session::~Session() {
    LOG << "Session exit, id=" << id_;
}

void Session::Open() {
    // Perform server-side handshake
    stream_.async_accept(asio::bind_executor(r_strand_, std::bind(
        &Session::OnOpened, this, std::placeholders::_1
    )));
}

void Session::Close() {
    // Send close message
    auto reason = ws::close_reason(ws::close_code::normal);
    stream_.async_close(reason, asio::bind_executor(w_strand_, std::bind(
        &Session::OnClosed, this, std::placeholders::_1
    )));
}

void Session::SendMessage(std::string const& message) {
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
    asio::post(w_strand_, std::bind(&Session::DoWrite, 
        this, data_size
    ));
}

void Session::OnOpened(error_code const& ec) {
    AsyncRead();
}

void Session::OnClosed(error_code const& ec) {
    if(ec) {
        LOG << "Close session failed: " << ec.message();
    } else {
        LOG << "Closing session ...";
    }
    host_->OnSessionClose(this);
}

void Session::AsyncRead() {
    stream_.async_read(r_buf_, asio::bind_executor(r_strand_, std::bind(
        &Session::OnRead, this,
        std::placeholders::_1, std::placeholders::_2
    )));
}

void Session::OnRead(error_code const& ec, size_t transferred_size) {
    if(ec) {
        if(ec == websocket_error::closed) {
            OnClosed(ec);
        } else if(ec != asio::error::operation_aborted) {
            LOG << "Unexcepted error on read: " << ec.message();
        }
        return;
    }
    // Handle incoming message
    if(stream_.got_text()) {
        auto message = boost::beast::buffers_to_string(r_buf_.data());
        host_->OnSessionMessage(this, message);
    }
    r_buf_.consume(transferred_size);
    // Read next message
    AsyncRead();
}

void Session::DoWrite(size_t data_size) {
    // Moving data from output buffer to input buffer
    w_buf_.commit(data_size);
    // Send text data
    stream_.text(true);
    error_code ec;
    auto written_size = stream_.write(w_buf_.data(), ec);
    // Error handling
    if(ec) {
        if(ec == websocket_error::closed) {
            OnClosed(ec);
        } else if(ec != asio::error::operation_aborted) {
            LOG << "Unexcepted error on write: " << ec.message();
        }
    } else {
        if(written_size != data_size) {
            LOG << "Incomplete writting: " 
                << written_size << "!=" << data_size;
        }
        w_buf_.consume(written_size);
    }
}

} // namespace websocket
} // namespace p2pd
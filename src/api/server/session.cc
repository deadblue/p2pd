#include "api/server/session.h"

#include <boost/beast/core/ostream.hpp>

#include "log/log.h"

namespace p2pd {
namespace api {

Session::Session(
    io_context & io_ctx, socket s, SessionHost * host
) : stream_(std::move(s)), host_(host), 
    r_strand_(io_ctx), w_strand_(io_ctx) {
    id_ = reinterpret_cast<uintptr_t>(this);
}

Session::~Session() {
    DLOG << "Session released, id=" << id_;
}

void Session::Open() {
    // Perform server-side handshake
    stream_.async_accept(asio::bind_executor(r_strand_, std::bind(
        &Session::OnOpened, 
        shared_from_this(), std::placeholders::_1
    )));
}

void Session::Close() {
    if(closed_) { return; }
    // Send close message
    auto reason = ws::close_reason(ws::close_code::normal);
    stream_.async_close(reason, asio::bind_executor(w_strand_, std::bind(
        &Session::OnClosed, 
        shared_from_this(), std::placeholders::_1
    )));
}

void Session::SendMessage(std::string message) {
    if(message.empty()) { return; }
    // Enqueueu writing task
    asio::post(w_strand_, std::bind(
        &Session::DoWrite, 
        shared_from_this(), std::move(message)
    ));
}

void Session::OnOpened(error_code const& ec) {
    // When handshake failed, close session.
    if(ec) {
        OnClosed(ec);
    } else {
        AsyncRead();
    }
}

void Session::OnClosed(error_code const& ec) {
    if(ec
        && ec != websocket_error::closed
        && ec != asio::error::operation_aborted
        && ec != asio::error::not_connected
    ) {
        WLOG << "Close session on error(" << ec.value()
            << "): " << ec.message();
    }
    if(!closed_.exchange(true)) {
        host_->OnSessionClose(id_);
    }
}

void Session::AsyncRead() {
    stream_.async_read(r_buf_, asio::bind_executor(r_strand_, std::bind(
        &Session::OnRead, shared_from_this(),
        std::placeholders::_1, std::placeholders::_2
    )));
}

void Session::OnRead(error_code const& ec, size_t transferred_size) {
    if(ec) {
        OnClosed(ec);
        return;
    }
    // Handle incoming message
    if(stream_.got_text()) {
        auto message = boost::beast::buffers_to_string(r_buf_.data());
        host_->OnSessionMessage(id_, std::move(message));
    }
    r_buf_.consume(transferred_size);
    // Read next message
    AsyncRead();
}

void Session::DoWrite(std::string message) {
    size_t msg_size = message.length();
    // Prepare buffer.
    buffer write_buf{};
    beast::ostream(write_buf) << message << std::flush;
    if(write_buf.size() != msg_size) {
        WLOG << "Incomplete buffer size: " 
            << write_buf.size() << "!=" << msg_size;
    }
    // Send text data
    stream_.text(true);
    error_code ec;
    auto written_size = stream_.write(write_buf.data(), ec);
    // Error handling
    if(ec) {
        // TODO: Retry on error?
        WLOG << "Write data error(" << ec.value()
            << "): " << ec.message();
    } else if(written_size != msg_size) {
        // TODO: Retry for incomplete transmission?
        WLOG << "Incomplete writting: " 
            << written_size << "!=" << msg_size;
    }
    write_buf.consume(msg_size);
}

} // namespace api
} // namespace p2pd
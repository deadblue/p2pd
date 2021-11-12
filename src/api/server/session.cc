#include "api/server/session.h"

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
    asio::post(w_strand_, std::bind(
        &Session::DoWrite, 
        shared_from_this(), data_size
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
        host_->OnSessionMessage(id_, message);
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
        // TODO: Retry on error?
        WLOG << "Write data error(" << ec.value()
            << "): " << ec.message();
    } else {
        // TODO: Retry for incomplete transmission?
        if(written_size != data_size) {
            WLOG << "Incomplete writting: " 
                << written_size << "!=" << data_size;
        }
    }
    w_buf_.consume(w_buf_.size());
}

} // namespace api
} // namespace p2pd
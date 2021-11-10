#include "api/server.h"

#include <boost/beast.hpp>
#include <mutex>

#include "common/logging.h"

namespace p2pd {
namespace api {

Server::Server(
    engine_ptr engine,
    io_context & io_ctx
) : io_ctx_(io_ctx), acceptor_(io_ctx) {
    ctrl_.reset(new Controller(engine, std::bind(
        &Server::PublishMessage, this, std::placeholders::_1
    )));
}

Server::~Server() {
    LOG << "API server destroyed!";
}

// ----- Override |p2pd::BasicServer| -----

void Server::Startup(const char * ip, uint16_t port) {
    auto endpoint = endpoint_type{ asio::ip::make_address(ip), port };

    // Start listening
    error_code ec;
    acceptor_.open(endpoint.protocol(), ec);
    if(ec) { LOG << "Open acceptor failed: " << ec.message(); }
    acceptor_.set_option(acceptor_type::reuse_address(true), ec);
    acceptor_.bind(endpoint, ec);
    if(ec) { LOG << "Bind acceptor failed: " << ec.message(); }
    acceptor_.listen(socket_type::max_listen_connections, ec);
    if(ec) { LOG << "Listen failed: " << ec.message(); }
    // Accept incoming connection
    DoAccept();
}

void Server::Shutdown() {
    // Do not accept new connecting request.
    error_code ec;
    acceptor_.cancel(ec);
    acceptor_.close(ec);
    // Close all still-alive sessions.
    if(sessions_.size() > 0) {
        LOG << "Wait for sessions closed ...";
        for(auto it = sessions_.begin(); it != sessions_.end(); ++it) {
            it->second->Close();
        }
        // Waiting for all session closed.
        std::mutex mutex;
        std::unique_lock<std::mutex> lock{mutex};
        cond_.wait(lock, [this]{
            return sessions_.size() == 0;
        });
        LOG << "All sessions closed, server is down!";
    }
}

// ----- Override |p2pd::api::ServerSessionHost| -----

void Server::OnSessionMessage(
    session_id id, std::string message
) {
    ctrl_->AsyncExecute(std::move(message), std::bind(
        &Server::SendMessageTo, 
        this, id, std::placeholders::_1
    ));
}

void Server::OnSessionClose(session_id id) {
    LOG << "Session closed, id=" << id;
    sessions_.erase(id);
    cond_.notify_all();
}

// ----- private methods -----

void Server::DoAccept() {
    if(!acceptor_.is_open()) { return; }
    acceptor_.async_accept(io_ctx_, std::bind(
        &Server::OnAccepted, this, 
        std::placeholders::_1, std::placeholders::_2
    ));
}

void Server::OnAccepted(error_code const& ec, socket_type socket) {
    if(ec) {
        if(ec != boost::asio::error::operation_aborted) {
            LOG << "Unexcepted error on accept, code=" << ec.value()
                << ", message=" << ec.message();
        }
    } else {
        // Create session
        auto session = std::make_unique<ServerSession>(
            std::move(socket), this, io_ctx_
        );
        auto id = session->id();
        session->Open();
        // Store session.
        sessions_[id] = std::move(session);
        // Accept next connection
        DoAccept();
    }
}

void Server::SendMessageTo(session_id id, std::string message) {
    if (sessions_.count(id) == 0) {
        LOG << "Can not send message to session: " << id;
        return;
    }
    auto & session = sessions_[id];
    session->SendMessage(std::move(message));
}

void Server::PublishMessage(std::string message) {
    for(auto & it : sessions_) {
        it.second->SendMessage(message);
    }
}

std::unique_ptr<Server> create_server(
    std::shared_ptr<p2pd::engine::Engine> engine, 
    io_context & io_ctx
) {
    auto server = std::make_unique<Server>(
        engine, io_ctx
    );
    return server;
}

} // namespace api
} // namespace p2pd
#include "websocket/server.h"

#include <boost/beast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <mutex>

#include "api/request.h"
#include "api/response.h"
#include "api/event.h"
#include "common/logging.h"
#include "json/binding.h"

namespace p2pd {
namespace websocket {

Server::Server(
    std::shared_ptr<p2pd::engine::Engine> engine,
    io_context & io_ctx
) : engine_(engine), 
    io_ctx_(io_ctx),
    acceptor_(io_ctx) {}

Server::~Server() {
    LOG << "Websocket API server closed!";
}

// ----- Override |p2pd::BasicServer| -----

void Server::Startup(uint16_t port) {
    auto host = asio::ip::make_address("0.0.0.0");
    auto endpoint = endpoint_type{ host, port };

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

// ----- Override |p2pd::websocket::SessionHost| -----

void Server::OnSessionMessage(
    Session * session, std::string message
) {
    // Parse incoming message as request 
    api::Request request;
    json::ParseAs(message, request);
    // TODO: Handle request
    LOG << "Request id=" << request.id
        << ", method=" << request.method;

    // TODO: Sending response
    api::Response response;
    response.id = request.id;
    response.error = false;
    auto result = json::ToString(response);
    session->SendMessage(result);
}

void Server::OnSessionClose(Session * session) {
    auto session_id = session->id();
    LOG << "Session closed, id=" << session_id;
    sessions_.erase(session_id);
    cond_.notify_all();
}

// ----- Override |p2pd::engine::Observer| -----

void Server::OnEngineAlert(std::string const& alert_message) {
    auto alert = api::event::EngineAlert();
    alert.message = alert_message;
    PublishEvent("engine.alert", std::move(alert));
}

void Server::OnTaskStateChanged(uint32_t task_id, engine::TaskState state) {
    // TODO: Publish event to client.
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
        auto session = std::make_unique<Session>(
            std::move(socket), this, io_ctx_
        );
        auto session_id = session->id();
        session->Open();
        // Store session.
        sessions_[session_id] = std::move(session);
        // Accept next connection
        DoAccept();
    }
}

template<typename T>
void Server::PublishEvent(std::string const& name, T && data) {
    // Make event message
    auto event = api::Event(name);
    boost::uuids::random_generator gen;
    event.id = boost::uuids::to_string(gen());
    event.data << std::move(data);
    auto message = json::ToString(std::move(event));
    // Publish to all sessions
    for (auto it = sessions_.begin(); it != sessions_.end(); ++it) {
        it->second->SendMessage(message);
    }
}

std::unique_ptr<Server> create_server(
    std::shared_ptr<p2pd::engine::Engine> engine, 
    io_context & io_ctx
) {
    auto server = std::make_unique<Server>(
        engine, io_ctx
    );
    engine->AddObserver(server.get());
    return server;
}

} // namespace websocket
} // namespace p2pd
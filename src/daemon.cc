#include "daemon.h"

#include <iostream>
#include <memory>

#include <libtorrent/version.hpp>

#include "api/server.h"
#include "engine/engine.h"
#include "log/log.h"
#include "version.h"

namespace p2pd {

using io_context = boost::asio::io_context;

static const char * version_str = 
    "p2pd " P2PD_VERSION " (libtorrent/" LIBTORRENT_VERSION ")";

static const char * banner = "\n"
    R"(            ___             __  )" "\n"
    R"(      ____ |__ \ ____  ____/ /  )" "\n"
    R"(     / __ \__/ // __ \/ __  /   )" "\n"
    R"(    / /_/ / __// /_/ / /_/ /    )" "\n"
    R"(   / .___/____/ .___/\__,_/     )" "\n"
    R"(  /_/        /_/                )" "\n";

// static
void Daemon::PrintVersion() {
    std::cout << version_str << std::endl;
}

Daemon::Daemon(
    Options const& options
) : options_(options) {}

int Daemon::Run() {
    // Print banner
    PrintBanner();

    // Register signal handler
    io_context io_ctx{1};
    signal_set signals{io_ctx, SIGINT, SIGTERM, SIGHUP};
    signals.async_wait(std::bind(&Daemon::OnSignal, 
        this, &signals, std::placeholders::_1, std::placeholders::_2
    ));

    // Create engine
    auto engine = engine::create();
    engine->Startup(options_);

    // Start server
    auto server = api::create_server(engine);
    server->Startup(options_.api_addr.c_str(), options_.api_port);
    LOG << "Server listening at " << options_.api_addr 
        << ":" << options_.api_port << " ...";

    // Waiting for signal handler exit.
    io_ctx.run();

    server->Shutdown();
    engine->Shutdown();
    return 0;
}

// ----- Private methods -----

void Daemon::PrintBanner() {
    std::cout << banner << std::endl;
}

void Daemon::OnSignal(
    signal_set * signals, 
    error_code const& ec, int signal
) {
    switch(signal) {
    case SIGINT: case SIGTERM: 
        LOG << "Receive exit signal(" 
            << signal << "), shutting down daemon ...";
        break;
    case SIGHUP:
        // TODO: Reload config.
        
        // Wait for next signal
        signals->async_wait(std::bind(&Daemon::OnSignal, 
            this, signals, std::placeholders::_1, std::placeholders::_2
        ));
        break;
    default: break;
    }
}

} // namespace p2pd
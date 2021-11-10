#include "daemon.h"

#include <condition_variable>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>

#include "api/server.h"
#include "common/logging.h"
#include "engine/engine.h"

namespace p2pd {

Daemon::Daemon(
    Options& options
) : options_(options), 
    io_ctx_(options_.pool_size), 
    signals_(io_ctx_, SIGINT, SIGTERM) {}

int Daemon::Run() {
    // Print banner
    PrintBanner();
    // Register signal handler
    AsyncWaitSignal();
    // Start thread pool
    LOG << "Starting workers, num = " << std::to_string(options_.pool_size);
    for(int i = 0; i < options_.pool_size; ++i) {
        std::thread([this]{
            io_ctx_.run();
        }).detach();
    }

    // Create engine
    auto engine = engine::Engine::Create();
    engine->Startup();
    // Start server
    auto server = api::create_server(engine, io_ctx_);
    server->Startup("0.0.0.0", options_.api_port);
    LOG << "Server listening at 0.0.0.0:" << options_.api_port << "...";

    std::mutex mutex;
    std::unique_lock<std::mutex> lock{mutex};
    cond_.wait(lock);
    LOG << "Receive exit signal, shutting down daemon ...";

    server->Shutdown();
    engine->Shutdown();
    io_ctx_.stop();
    return 0;
}

// ----- Private methods -----

void Daemon::PrintBanner() {
    const char * banner = "\n"
        R"(            ___             __  )" "\n"
        R"(      ____ |__ \ ____  ____/ /  )" "\n"
        R"(     / __ \__/ // __ \/ __  /   )" "\n"
        R"(    / /_/ / __// /_/ / /_/ /    )" "\n"
        R"(   / .___/____/ .___/\__,_/     )" "\n"
        R"(  /_/        /_/                )" "\n";
    std::cout << banner << std::endl;
}

void Daemon::AsyncWaitSignal() {
    signals_.async_wait(std::bind(&Daemon::OnSignal, 
        this, std::placeholders::_1, std::placeholders::_2
    ));
}

void Daemon::OnSignal(const error_code& ec, int signal) {
    switch (signal) {
    case SIGINT: 
    case SIGTERM:
        cond_.notify_one();
        break;
    default:
        AsyncWaitSignal();
        break;
    }
}

} // namespace p2pd
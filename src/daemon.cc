#include "daemon.h"

#include <condition_variable>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>

#include "api/server.h"
#include "engine/engine.h"
#include "log/log.h"

namespace p2pd {

using signal_set = boost::asio::signal_set;

Daemon::Daemon(
    Options const& options
) : options_(options), io_ctx_(options_.pool_size){}

int Daemon::Run() {
    // Print banner
    PrintBanner();

    // Register signal handler
    std::condition_variable cond{};
    signal_set signals{io_ctx_, SIGINT, SIGTERM};
    signals.async_wait(std::bind(&Daemon::OnSignal, 
        this, &cond, std::placeholders::_1, std::placeholders::_2
    ));

    // Start thread pool
    DLOG << "Starting workers, num = " << std::to_string(options_.pool_size);
    for(int i = 0; i < options_.pool_size; ++i) {
        std::thread([this]{
            io_ctx_.run();
        }).detach();
    }

    // Create engine
    auto engine = engine::create(options_);
    engine->Startup();
    // Start server
    auto server = api::create_server(engine, io_ctx_);
    server->Startup("0.0.0.0", options_.api_port);
    LOG << "Server listening at 0.0.0.0:" << options_.api_port << "...";

    std::mutex mutex;
    std::unique_lock<std::mutex> lock{mutex};
    cond.wait(lock);
    DLOG << "Receive exit signal, shutting down daemon ...";

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

void Daemon::OnSignal(
    std::condition_variable * cond, 
    error_code const& ec, int signal
) {
    cond->notify_one();
}

} // namespace p2pd
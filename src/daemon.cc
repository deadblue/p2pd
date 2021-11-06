#include "daemon.h"

#include <condition_variable>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>

#include "common/logging.h"
#include "engine/engine.h"
#include "websocket/server.h"

namespace p2pd {

void print_banner() {
    const char * banner = "\n"
        R"(            ___             __  )" "\n"
        R"(      ____ |__ \ ____  ____/ /  )" "\n"
        R"(     / __ \__/ // __ \/ __  /   )" "\n"
        R"(    / /_/ / __// /_/ / /_/ /    )" "\n"
        R"(   / .___/____/ .___/\__,_/     )" "\n"
        R"(  /_/        /_/                )" "\n";
    std::cout << banner << std::endl;
}

Daemon::Daemon(
    size_t pool_size
) : pool_size_(pool_size), 
    io_ctx_(pool_size), 
    signals_(io_ctx_, SIGINT, SIGTERM) {}

int Daemon::Run() {
    print_banner();
    // Register signal handler
    AsyncWaitSignal();
    // Start thread pool
    LOG << "Starting workers, num=" << pool_size_;
    for(int i = 0; i < pool_size_; ++i) {
        std::thread(std::bind(
            &Daemon::Worker, this
        )).detach();
    }

    // Start engine
    auto engine = std::make_shared<engine::Engine>();
    // Start server
    auto server = std::make_unique<websocket::Server>(
        engine, io_ctx_
    );
    server->Startup(9066);
    LOG << "Server listening at :9066 ...";

    std::mutex mutex;
    std::unique_lock<std::mutex> lock{mutex};
    cond_.wait(lock);
    LOG << "Receive exit signal, shutting down server ...";

    server->Shutdown();
    io_ctx_.stop();
    LOG << "Bye-bye!";
    return 0;
}

void Daemon::Worker() {
    LOG << "Worker thread starting, id=" << std::this_thread::get_id();
    io_ctx_.run();
    LOG << "Worker thread exited, id=" << std::this_thread::get_id();
}

void Daemon::AsyncWaitSignal() {
    signals_.async_wait(std::bind(&Daemon::OnSignal, 
        this, std::placeholders::_1, std::placeholders::_2
    ));
}

void Daemon::OnSignal(const error_code& ec, int signal) {
    if(SIGINT == signal) {
        cond_.notify_one();
    } else {
        AsyncWaitSignal();
    }
}

} // namespace p2pd
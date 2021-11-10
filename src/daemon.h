#ifndef P2PD_DAEMON_H
#define P2PD_DAEMON_H

#include <memory>
#include <condition_variable>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "options.h"

namespace p2pd {

class Daemon {

private:
    using io_context = boost::asio::io_context;
    using error_code = boost::system::error_code;

    Options const& options_;
    io_context io_ctx_;

public:
    // Constructor
    Daemon(Options const& options);

    // Run the daemon, return exit code when exit.
    int Run();

private:
    void PrintBanner();
    void OnSignal(
        std::condition_variable * cond, 
        error_code const& ec, int signal
    );

};

} // namespace p2pd

#endif // P2PD_DAEMON_H

#ifndef P2PD_DAEMON_H
#define P2PD_DAEMON_H

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "options.h"

namespace p2pd {

class Daemon {

private:
    using signal_set = boost::asio::signal_set;
    using error_code = boost::system::error_code;

    Options const& options_;

public:
    // Print daemon version.
    static void PrintVersion();

    // Constructor
    Daemon(Options const& options);

    // Run the daemon, return exit code when exit.
    int Run();

private:
    void PrintBanner();
    void OnSignal(
        signal_set * signals, 
        error_code const& ec, int signal
    );

};

} // namespace p2pd

#endif // P2PD_DAEMON_H

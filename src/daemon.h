#ifndef P2PD_APPLICATION_H
#define P2PD_APPLICATION_H

#include <memory>
#include <condition_variable>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "options.h"

namespace p2pd {

class Daemon {

using error_code = boost::system::error_code;

private:
    Options& options_;
    boost::asio::io_context io_ctx_;
    boost::asio::signal_set signals_;
    std::condition_variable cond_{};

public:
    // Constructor
    Daemon(Options& options);
    int Run();

private:
    void PrintBanner();
    void AsyncWaitSignal();
    void OnSignal(error_code const& ec, int signal);

};

} // namespace p2pd

#endif // P2PD_APPLICATION_H

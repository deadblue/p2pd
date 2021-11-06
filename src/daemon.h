#ifndef P2PD_APPLICATION_H
#define P2PD_APPLICATION_H

#include <memory>
#include <condition_variable>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "base_server.h"

namespace p2pd {

class Daemon {

using error_code = boost::system::error_code;

private:
    size_t pool_size_;
    boost::asio::io_context io_ctx_;
    boost::asio::signal_set signals_;
    std::condition_variable cond_{};

public:
    Daemon(size_t pool_size);
    int Run();

private:
    void Worker();
    void AsyncWaitSignal();
    void OnSignal(const error_code& ec, int signal);

};

} // namespace p2pd

#endif // P2PD_APPLICATION_H

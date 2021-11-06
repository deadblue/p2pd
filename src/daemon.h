#ifndef P2PD_APPLICATION_H
#define P2PD_APPLICATION_H

#include <memory>
#include <condition_variable>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace p2pd {

// namespace shortcut
namespace asio = boost::asio;
// type shortcut
using error_code = boost::system::error_code;

class Daemon {

private:
    size_t pool_size_;
    asio::io_context io_ctx_;
    asio::signal_set signals_;
    std::condition_variable cond_{};

public:
    // Constructor
    Daemon(size_t pool_size);
    int Run();

private:
    void PrintBanner();
    void AsyncWaitSignal();
    void OnSignal(const error_code& ec, int signal);

};

} // namespace p2pd

#endif // P2PD_APPLICATION_H

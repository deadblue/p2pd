#ifndef P2PD_BASE_SERVER_H
#define P2PD_BASE_SERVER_H

#include <cstdint>
#include <memory>
#include <boost/asio.hpp>

#include "engine/engine.h"

namespace p2pd {

class BaseServer {

private:
    std::shared_ptr<engine::Engine> engine_;
protected:
    boost::asio::io_context & io_ctx_;

public:
    explicit BaseServer(
        std::shared_ptr<engine::Engine> engine,
        boost::asio::io_context & io_ctx
    ): engine_(engine), io_ctx_(io_ctx) {}
    virtual ~BaseServer() = default;

    virtual void Startup(uint16_t port) = 0;
    virtual void Shutdown() = 0;
};

} // namespace p2pd

#endif // P2PD_BASE_SERVER_H

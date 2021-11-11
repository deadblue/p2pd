#ifndef P2PD_API_SERVER_TYPE_H
#define P2PD_API_SERVER_TYPE_H

#include <cstdint>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace p2pd {
namespace api {

// ----- Namespaces -----
namespace asio  = boost::asio;
namespace beast = boost::beast;
namespace ws    = boost::beast::websocket;

// ----- Types -----
using error_code = boost::system::error_code;

using io_context = asio::io_context;
using strand     = asio::io_context::strand;

using acceptor = boost::asio::ip::tcp::acceptor;
using endpoint = boost::asio::ip::tcp::endpoint;
using socket   = boost::asio::ip::tcp::socket;

using stream = ws::stream<socket>;
using buffer = beast::multi_buffer;
using websocket_error = ws::error;

} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVER_TYPE_H
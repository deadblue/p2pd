#include "api/service/add_task.h"
#include "api/protocol.h"
#include "json/io.h"
#include "log/log.h"

#include <boost/beast/core/detail/base64.hpp>

namespace p2pd {
namespace api {
namespace service {

int AddTask::Execute(json::Node const& params, json::Node & result) {
    auto rp = params >> api::request::AddTask();
    auto ec = error_code{};
    if(rp.type == "magnet") {
        AddMagnet(rp.uri, ec);
    } else if(rp.type == "torrent") {
        AddTorrent(rp.uri, ec);
    }
    return ec ? ec.value() : 0;
}

void AddTask::AddMagnet(std::string & uri, error_code & ec) {
    engine_->AddMagnet(uri.c_str(), ec);
    if(ec) {
        WLOG << "Add torrent task error("<< ec.value() 
            << "): " << ec.message();
    }
}

void AddTask::AddTorrent(std::string & data, error_code & ec) {
    size_t src_size = data.size();
    if(src_size % 4 != 0) {
        WLOG << "Invalid input data!";
        return;
    }
    // Decode torrent data
    size_t dest_size = src_size / 4 * 3;
    if(data.at(src_size - 1) == '=') {
        dest_size -= 1;
        if(data.at(src_size - 2) == '=') {
            dest_size -= 1;
        }
    }
    auto buf = std::make_unique<uint8_t>(malloc(dest_size));
    boost::beast::detail::base64::decode(
        buf.get(), data.c_str(), src_size
    );
    // Add torrent
    engine_->AddTorrent(buf.get(), dest_size, ec);
    if(ec) {
        WLOG << "Add torrent task error("<< ec.value() 
            << "): " << ec.message();
    }
}

} // namespace service
} // namespace api
} // namespace p2pd
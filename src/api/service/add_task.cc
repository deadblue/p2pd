#include "api/service/add_task.h"
#include "json/io.h"
#include "json/macro.h"
#include "log/log.h"

#include <boost/beast/core/detail/base64.hpp>

namespace p2pd {

namespace json {

DEFINE_UNMARSHALLER(api::service::AddTaskParams,
    UNMARSHAL_FIELD(type)
    UNMARSHAL_FIELD(uri)
)

DEFINE_MARSHALLER(api::service::AddTaskResult,
    MARSHAL_FIELD(task_id)
)

} // namespace json

namespace api {
namespace service {

int AddTask::Execute(json::Node const& params, json::Node & result) {
    auto sp = params >> AddTaskParams();
    auto ec = error_code{};
    if(sp.type == "magnet") {
        AddMagnet(sp.uri, ec);
    } else if(sp.type == "torrent") {
        AddTorrent(sp.uri, ec);
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
    auto dest = (uint8_t *)malloc(dest_size);
    boost::beast::detail::base64::decode(
        dest, data.c_str(), src_size
    );
    // Add torrent
    engine_->AddTorrent(dest, dest_size, ec);
    free(dest);
    if(ec) {
        WLOG << "Add torrent task error("<< ec.value() 
            << "): " << ec.message();
    }
}

} // namespace service
} // namespace api

} // namespace p2pd
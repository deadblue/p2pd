#include "api/service/add_task.h"
#include "api/protocol.h"
#include "json/io.h"

namespace p2pd {
namespace api {
namespace service {

int AddTask::Execute(json::Node const& params, json::Node & result) {
    auto rp = params >> api::request::AddTask();
    if(rp.type == "magnet") {
        AddMagnet(rp.uri);
    } else if(rp.type == "torrent") {
        AddTorrent(rp.uri);
    }
    return 0;
}

void AddTask::AddMagnet(std::string & uri) {
    engine_->AddMagnet(uri.c_str());
}

void AddTask::AddTorrent(std::string & data) {
    // TODO: Base64 decode data
}

} // namespace service
} // namespace api
} // namespace p2pd
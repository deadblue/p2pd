#include "engine/task_plugin.h"

namespace p2pd {
namespace engine {

TaskPlugin::TaskPlugin(
    uint32_t id, std::shared_ptr<TaskHost> host
) : id_(id), host_(host) {}

void TaskPlugin::on_state(lt::torrent_status::state_t state) {
    if(!host_) { return; }
    host_->OnTaskStateChanged(id_, static_cast<int>(state));
}

} // namespace engine
} // namespace p2pd
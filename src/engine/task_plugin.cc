#include "engine/task_plugin.h"

namespace p2pd {
namespace engine {

TaskPlugin::TaskPlugin(
    uint32_t id, std::shared_ptr<TaskHost> host
) : id_(id), host_(host) {}

bool TaskPlugin::on_pause() {
    return true;
}

bool TaskPlugin::on_resume() {
    return true;
}

void TaskPlugin::on_state(lt::torrent_status::state_t state) {
    if(!host_) { return; }
    // Forward state to host
    host_->OnTaskStateChanged(id_, state);
}

} // namespace engine
} // namespace p2pd
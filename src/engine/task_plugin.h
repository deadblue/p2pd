#ifndef P2PD_ENGINE_PLUGIN_H
#define P2PD_ENGINE_PLUGIN_H

#include <memory>
#include <libtorrent/extensions.hpp>

#include "engine/task_host.h"

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

class TaskPlugin 
    : public lt::torrent_plugin {

private:
    uint32_t id_;
    std::shared_ptr<TaskHost> host_;

public:
    TaskPlugin(uint32_t id, std::shared_ptr<TaskHost> host);
    // |libtorrent::torrent_plugin| implementation
    void on_state(lt::torrent_status::state_t state);

};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_PLUGIN_H
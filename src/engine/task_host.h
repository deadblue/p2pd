#ifndef P2PD_ENGINE_TASK_HOST_H
#define P2PD_ENGINE_TASK_HOST_H

#include <libtorrent/torrent_status.hpp>
#include <cstdint>

namespace p2pd {
namespace engine {

class TaskHost {

protected:
    using task_state = libtorrent::torrent_status::state_t;

public:
    // Fired when task is paused.
    virtual void OnTaskPause(uint32_t task_id) {}
    // Fired when task is resuming.
    virtual void OnTaskResume(uint32_t task_id) {}
    // Fired when task state is changed
    virtual void OnTaskStateChanged(
        uint32_t task_id, task_state state
    ) {}

};

} // namespace engine
} // namespace bt

#endif // P2PD_ENGINE_TASK_HOST_H
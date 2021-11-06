#ifndef P2PD_ENGINE_TASK_HOST_H
#define P2PD_ENGINE_TASK_HOST_H

#include <cstdint>

namespace p2pd {
namespace engine {

class TaskHost {
public:
    virtual void OnTaskStateChanged(
        uint32_t task_id, int state
    ) {}
};

} // namespace engine
} // namespace bt

#endif // P2PD_ENGINE_TASK_HOST_H
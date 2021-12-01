#ifndef P2PD_API_EVENT_H
#define P2PD_API_EVENT_H

#include <string>

namespace p2pd {
namespace api {
namespace event {

struct TaskStateUpdated {
    std::string task_id;
    int old_state;
    int new_state;

    TaskStateUpdated() = default;
    TaskStateUpdated(
        std::string task_id, int old_state, int new_state
    ) : task_id(std::move(task_id)), 
        old_state(old_state), 
        new_state(new_state) {}
};

} // namespace event
} // namespace api
} // namespace p2pd

#endif // P2PD_API_EVENT_H
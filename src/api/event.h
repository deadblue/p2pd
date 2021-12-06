#ifndef P2PD_API_EVENT_H
#define P2PD_API_EVENT_H

#include <string>

namespace p2pd {
namespace api {
namespace event {

struct TaskStateUpdated {
    std::string id;
    int old_state;
    int new_state;

    TaskStateUpdated() = default;
    TaskStateUpdated(
        std::string id, int old_state, int new_state
    ) : id(std::move(id)), 
        old_state(old_state), 
        new_state(new_state) {}
};

} // namespace event
} // namespace api
} // namespace p2pd

#endif // P2PD_API_EVENT_H
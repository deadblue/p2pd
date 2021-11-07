#ifndef P2PD_API_EVENT_H
#define P2PD_API_EVENT_H

#include <cstdint>
#include <string>

#include "json/node.h"

namespace p2pd {
namespace api {

struct Event {
    std::string id;
    std::string name;
    json::Node data;

    Event() = default;
    Event(std::string const& name) : name(name) {}
};

namespace event {

struct EngineAlert {
    std::string message;
};

struct TaskStateChanged {
    uint32_t task_id;
    int state;
};

} // namespace event
} // namespace api
} // namespace p2pd

#endif // P2PD_API_EVENT_H
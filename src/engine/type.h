#ifndef P2PD_ENGINE_TYPE_H
#define P2PD_ENGINE_TYPE_H

#include <cstdint>
#include <string>

namespace p2pd {
namespace engine {

enum TaskState {
    preparing,
    downloading,
    uploading,
    finished
};

struct Task {
    std::string task_id;
    std::string name;
    uint64_t size;
};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_TYPE_H
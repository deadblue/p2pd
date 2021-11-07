#ifndef P2PD_ENGINE_TYPE_H
#define P2PD_ENGINE_TYPE_H

namespace p2pd {
namespace engine {

enum TaskState {
    preparing,
    downloading,
    seeding,
    finished
};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_TYPE_H
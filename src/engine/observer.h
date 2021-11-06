#ifndef P2PD_ENGINE_OBSERVER_H
#define P2PD_ENGINE_OBSERVER_H

#include <cstdint>

namespace p2pd {
namespace engine {

/**
 * Observer can receive download events from engine.
 */
class Observer{

    virtual void OnTaskAdd() {}

    virtual void OnTaskStateChanged(uint32_t task_id) {}
    
};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_OBSERVER_H
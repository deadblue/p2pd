#ifndef P2PD_ENGINE_OBSERVER_H
#define P2PD_ENGINE_OBSERVER_H

#include <cstdint>
#include <string>

#include "engine/type.h"

namespace p2pd {
namespace engine {

/**
 * Observer can receive download events from engine.
 */
class Observer{

public:
    /**
     * Fired when engine dispatch an alert.
     */
    virtual void OnEngineAlert(std::string const& message) {}

    /**
     * Fired when state of a task is changed.
     */
    virtual void OnTaskStateChanged(uint32_t task_id, TaskState state) {}

    // TODO: Declare more observer methods.
    
};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_OBSERVER_H
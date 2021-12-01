#ifndef P2PD_ENGINE_OBSERVER_H
#define P2PD_ENGINE_OBSERVER_H

#include <cstdint>
#include <string>

#include "engine/type.h"

namespace p2pd {
namespace engine {

/**
 * @brief   Observer which receives events from engine.
 * 
 * @author  deadblue
 */
class Observer {

public:

    /**
     * @brief   Called when a new task is created in engine.
     */
    virtual void OnTaskCreated(TaskMetadata const& metadata) const {}

    /**
     * @brief   Called when a task has received metadata from other nodes, only 
     *          when a task is added without metadata (e.g. by magnet URI).
     */
    virtual void OnTaskMetadataReceived(TaskMetadata const& metadata) const {}

    /**
     * @brief   Called when task state changed.
     */
    virtual void OnTaskStateChanged(
        std::string const& task_id, 
        TaskSummary::State old_state,
        TaskSummary::State new_state
    ) const {}

    // TODO: Declare more observer methods.
    
};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_OBSERVER_H
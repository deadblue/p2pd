#ifndef P2PD_ENGINE_ENGINE_H
#define P2PD_ENGINE_ENGINE_H

#include <cstdint>
#include <map>

#include "engine/error_code.h"
#include "engine/observer.h"
#include "engine/type.h"
#include "options.h"

namespace p2pd {
namespace engine {

class Engine {
public:
    // Destructor
    virtual ~Engine() = default;;

    void AddObserver(Observer * ob) noexcept {
        uintptr_t key = reinterpret_cast<uintptr_t>(ob);
        if(observers_.count(key) == 0) {
            observers_.emplace(key, ob);
        }
    }
    void RemoveObserver(Observer * ob) noexcept {
        uintptr_t key = reinterpret_cast<uintptr_t>(ob);
        if(observers_.count(key) > 0) {
            observers_.erase(key);
        }
    }

    // APIs
    virtual void Startup(Options const& options) = 0;
    virtual void Shutdown() = 0;
    
    virtual void AddMagnet(const char * uri, error_code & ec) = 0;
    virtual void AddTorrent(
        const uint8_t * data, size_t data_size, error_code & ec
    ) = 0;
    virtual void ListTask(std::vector<TaskSummary> & summaries) = 0;
    /**
     * @brief Retrieve task metadata.
     */
    virtual void RetrieveTask(
        std::string const& task_id, TaskMetadata & metadata, 
        error_code & ec
    ) = 0;
    /**
     * @brief Inspect task status
     */
    virtual void InspectTask(
        std::string const& task_id, TaskStatus & status, 
        error_code & ec
    ) = 0;
    // TODO: Add more APIs.

protected:
    std::map<uintptr_t, Observer *> observers_{};

};

std::shared_ptr<Engine> create();

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_ENGINE_H
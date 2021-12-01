#ifndef P2PD_ENGINE_ENGINE_IMPL_H
#define P2PD_ENGINE_ENGINE_IMPL_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <libtorrent/extensions.hpp>

#include "options.h"
#include "engine/engine.h"
#include "engine/settings.h"

namespace libtorrent {
struct add_torrent_params;
struct session;
}

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

/**
 * @brief The BT implementation of Engine.
 * 
 * @author deadblue
 */
class EngineImpl final : public Engine, 
                         public lt::plugin, 
                         public std::enable_shared_from_this<EngineImpl> {

private:
    lt::session * session_;
    Settings settings_{};

public:
    // Constructor
    EngineImpl() = default;
    // Destructor
    ~EngineImpl() override;

    // Override |Engine|
    void Startup(Options const& options) final;
    void Shutdown() final;

    void AddMagnet(const char * uri, error_code & ec) final;
    void AddTorrent(
        const uint8_t * data, size_t data_size, error_code & ec
    ) final;
    void ListTask(std::vector<TaskSummary> & summaries) final;
    void RetrieveTask(
        std::string const& task_id, TaskMetadata & metadata, 
        error_code & ec
    ) final;
    void InspectTask(
        std::string const& task_id, TaskStatus & status, 
        error_code & ec
    ) final;

    // Override |libtorrent::plugin|
    lt::feature_flags_t implemented_features() final;
    void on_alert(lt::alert const* alert) final;

private:
    void AddTaskInternal(lt::add_torrent_params params);

};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_ENGINE_IMPL_H

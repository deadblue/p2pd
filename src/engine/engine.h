#ifndef P2PD_ENGINE_H
#define P2PD_ENGINE_H

#include <memory>
#include <vector>

#include <libtorrent/extensions.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/version.hpp>

#include "engine/task_host.h"
#include "engine/observer.h"

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

using error_code = lt::error_code;

/**
 * @brief The core P2P download engine.
 * 
 * @author deadblue
 */
class Engine :  public std::enable_shared_from_this<Engine>, 
                public lt::plugin,
                public TaskHost {

private:
#if LIBTORRENT_VERSION_MAJOR > 1
    using client_data_type = lt::client_data_t;
#else
    using client_data_type = void *;
#endif

    lt::session * session_;
    std::vector<Observer *> observers_{};

public:
    // Constructor
    Engine();
    // Destructor
    ~Engine();

    // ----- Public API -----
    static std::shared_ptr<Engine> Create();
    void Startup();
    void Shutdown();
    void AddObserver(Observer * observer);

    void AddMagnet(const char * uri, error_code & ec);
    void AddTorrent(const unsigned char * data, size_t data_size);
    void ListTask();

    // Override |libtorrent::plugin|
    lt::feature_flags_t implemented_features() final;
    std::shared_ptr<lt::torrent_plugin> new_torrent(
        lt::torrent_handle const& handle, 
        client_data_type client_data
    ) final;
    void on_alert(lt::alert const* alert) final;

    // Override |TaskHost|
    void OnTaskStateChanged(
        uint32_t torrent_id, task_state state
    ) final;

};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_H

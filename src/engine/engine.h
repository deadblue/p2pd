#ifndef P2PD_ENGINE_H
#define P2PD_ENGINE_H

#include <memory>
#include <vector>

#include <libtorrent/session.hpp>
#include <libtorrent/extensions.hpp>

#include "engine/task_host.h"
#include "engine/observer.h"

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

class Engine
    : public std::enable_shared_from_this<Engine>, 
      public lt::plugin,
      public TaskHost {

private:
    std::unique_ptr<lt::session> session_;
    std::vector<Observer*> observers_{};

public:
    Engine();
    ~Engine();

    // Public API
    void AddObserver(Observer * observer);
    void AddTorrent(const unsigned char * data, size_t data_size);
    void AddMagnet(const char * magnet_uri);
    void ListTask();
    // Override |libtorrent::plugin|
    lt::feature_flags_t implemented_features() final;
    std::shared_ptr<lt::torrent_plugin> new_torrent(
        lt::torrent_handle const& handle, 
        void * client_data
    ) final;
    void on_alert(lt::alert const* alert) final;
    // Override |TaskHost|
    void OnTaskStateChanged(
        uint32_t torrent_id, int state
    ) final;

};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_H

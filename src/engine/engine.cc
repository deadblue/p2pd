#include "engine/engine.h"

#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>

#include "common/logging.h"
#include "engine/settings.h"
#include "engine/task_plugin.h"

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

Engine::Engine() {
    session_ = std::make_unique<lt::session>();
}

Engine::~Engine() {
    LOG << "P2P Engine exit!";
}

// ----- Public API -----

void Engine::AddObserver(Observer * observer) {
    observers_.push_back(observer);
}

void Engine::AddTorrent(const unsigned char * data, size_t data_size) {
    auto params = lt::add_torrent_params{};
    params.ti = std::make_shared<lt::torrent_info>(
        reinterpret_cast<const char*>(data),  data_size
    );
    session_->add_torrent(params);
}

void Engine::AddMagnet(const char * magnet_uri) {
    auto params = lt::parse_magnet_uri(magnet_uri);
    session_->add_torrent(params);
}

void Engine::ListTask() {
    auto handles = session_->get_torrents();
    for(auto & handle : handles) {
    }
}

// ----- Override |libtorrent::plugin| -----

lt::feature_flags_t Engine::implemented_features() {
    lt::feature_flags_t flags{ lt::plugin::alert_feature };
    return flags;
}

std::shared_ptr<lt::torrent_plugin> Engine::new_torrent(
    lt::torrent_handle const& th, void * user_data
) {
    return std::make_shared<TaskPlugin>(
        th.id(), shared_from_this()
    );
}

void Engine::on_alert(lt::alert const* alert) {
    LOG << "Alert: " << alert->message();
    // TODO: Send alert to client.
    for(auto const& observer : observers_) {
        observer->OnAlert(alert->message());
    }
}

// ----- Override |libtorrent::TaskHost| -----

void Engine::OnTaskStateChanged(uint32_t task_id, int state) {
    // TODO: Send torrent state to client.
}

} // namespace engine
} // namespace p2pd 
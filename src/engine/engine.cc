#include "engine/engine.h"

#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>

#include "common/logging.h"
#include "engine/settings.h"
#include "engine/task_plugin.h"
#include "version.h"

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

Engine::Engine() {}

Engine::~Engine() {
    LOG << "P2P Engine exit!";
}

// ----- Public API -----

// static
std::shared_ptr<Engine> Engine::Create() {
    return std::make_shared<Engine>();
}

void Engine::Startup() {
    // 
    auto settings = lt::default_settings();
    settings.set_str(
        lt::settings_pack::user_agent, "p2pd/" P2PD_VERSION
    );
    // Create session
    session_ = new lt::session(
        std::move(settings)
    );
    session_->add_extension(shared_from_this());
}

void Engine::Shutdown() {
    delete session_;
}

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
    for(auto const& observer : observers_) {
        observer->OnEngineAlert(alert->message());
    }
}

// ----- Override |libtorrent::TaskHost| -----

void Engine::OnTaskStateChanged(uint32_t task_id, task_state task_state) {
    TaskState state;
    switch (task_state) {
    case lt::torrent_status::state_t::checking_files:
    case lt::torrent_status::state_t::checking_resume_data:
    case lt::torrent_status::state_t::downloading_metadata:
        state = TaskState::preparing;
        break;
    case lt::torrent_status::state_t::downloading:
        state = TaskState::downloading;
        break;
    case lt::torrent_status::state_t::seeding:
        state = TaskState::seeding;
        break;
    case lt::torrent_status::state_t::finished:
        state = TaskState::finished;
        break;
    default:
        break;
    }
    for(auto const& observer : observers_) {
        observer->OnTaskStateChanged(task_id, state);
    }
}

} // namespace engine
} // namespace p2pd 
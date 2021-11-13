#include "engine/engine.h"

#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/alert.hpp>

#include "engine/settings.h"
#include "engine/task_plugin.h"
#include "log/log.h"
#include "version.h"

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

Engine::Engine() {}

Engine::~Engine() {
    DLOG << "P2P Engine destroyed!";
}

// ----- Public API -----

void Engine::Startup() {
    // Make settings
    auto settings = lt::default_settings();
    settings.set_str(
        lt::settings_pack::user_agent, 
            "p2pd/" P2PD_VERSION 
            " libtorrent/" LIBTORRENT_VERSION
    );
    settings.set_str(
        lt::settings_pack::peer_fingerprint, 
        lt::generate_fingerprint("PD", 
            P2PD_VERSION_MAJOR, 
            P2PD_VERSION_MINOR,
            P2PD_VERSION_PATCH
        )
    );
    settings.set_bool(
        lt::settings_pack::enable_dht, true
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

void Engine::AddMagnet(const char * uri, error_code & ec) {
    auto params = lt::parse_magnet_uri(uri, ec);
    if(ec) {
        WLOG << "Parse magnet URI error(" << ec.value() 
            << "): " << ec.message();
        return;
    }
    params.save_path = settings_.save_dir;
    if( params.trackers.empty()  && !settings_.trackers.empty() ) {
        for (auto const& tracker : settings_.trackers) {
            params.trackers.push_back(tracker);
        }
    }
    // Add torrent to session
    auto handle = session_->add_torrent(std::move(params), ec);
    if(ec) {
        WLOG << "Add BT task error(" << ec.value() 
            << "): " << ec.message();
        return;
    }
}

void Engine::AddTorrent(
    const unsigned char * data, size_t data_size, error_code & ec
) {
    auto params = lt::add_torrent_params{};
    params.ti = std::make_shared<lt::torrent_info>(
        reinterpret_cast<const char*>(data),  data_size
    );
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
    lt::torrent_handle const& th, client_data_type user_data
) {
    return std::make_shared<TaskPlugin>(
        th.id(), shared_from_this()
    );
}

void Engine::on_alert(lt::alert const* alert) {
    for(auto * observer : observers_) {
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
        state = TaskState::uploading;
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

std::shared_ptr<Engine> create(Options const& options) {
    auto ptr = std::make_shared<Engine>();
    // Fill settings in engine
    ptr->settings_.save_dir = options.save_dir;
    return ptr;
}

} // namespace engine
} // namespace p2pd 
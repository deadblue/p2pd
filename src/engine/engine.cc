#include "engine/engine.h"

#include <fstream>
#include <sstream>

#include <libtorrent/alert.hpp>
#include <libtorrent/info_hash.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/session.hpp>

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

void fill_settings(Options const& options, Settings & settings) {
    if(!options.save_dir.empty()) {
        settings.save_dir = options.save_dir;
    }
    if(!options.tracker_list.empty()) {
        std::ifstream ifs{options.tracker_list};
        if(ifs) {
            for(std::string line; std::getline(ifs, line); ) {
                if(line.empty() || line[0] == '#') continue;
                DLOG << "Add tracker: " << line;
                settings.trackers.push_back(line);
            }
            ifs.close();
        }
    }
}

lt::session_params make_session_params(Options const& options) {
    auto settings = lt::default_settings();
    settings.set_str(lt::settings_pack::user_agent, 
        "p2pd/" P2PD_VERSION " (libtorrent/" LIBTORRENT_VERSION ")"
    );
    settings.set_str(lt::settings_pack::peer_fingerprint,
        lt::generate_fingerprint("PD", 
            P2PD_VERSION_MAJOR, 
            P2PD_VERSION_MINOR, 
            P2PD_VERSION_PATCH
        )
    );
    // Listen port
    std::string listen_addr{"0.0.0.0:"};
    listen_addr.append(std::to_string(options.bt_port));
    DLOG << "BT Listen address: " << listen_addr;
    settings.set_str(lt::settings_pack::listen_interfaces,
        listen_addr
    );
    // Announce IP
    if(!options.ip.empty()) {
        settings.set_str(lt::settings_pack::announce_ip, 
            options.ip
        );
    }
    // DHT bootstrap nodes
    if(!options.dht_node_list.empty()) {
        std::ifstream ifs{options.dht_node_list};
        if(ifs) {
            bool first = true;
            std::ostringstream oss;
            for(std::string line; std::getline(ifs, line); ) {
                if(first) {
                    first = false;
                } else {
                    oss << ",";
                }
                oss << line;
            }
            ifs.close();
            settings.set_str(lt::settings_pack::dht_bootstrap_nodes,
                oss.str()
            );
        }
    }
    settings.set_bool(lt::settings_pack::enable_dht, true);

    return lt::session_params(std::move(settings));
}

void Engine::Startup(Options const& options) {
    fill_settings(options, settings_);
    // Create session
    auto params = make_session_params(options);
    params.extensions.push_back(shared_from_this());
    session_ = new lt::session(std::move(params));
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
    params.flags ^= lt::torrent_flags::paused;
    AddTask(std::move(params), ec);
}

void Engine::AddTorrent(
    const unsigned char * data, size_t data_size, error_code & ec
) {
    auto params = lt::add_torrent_params{};
    params.ti = std::make_shared<lt::torrent_info>(
        reinterpret_cast<const char*>(data),  data_size
    );
    AddTask(std::move(params), ec);
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

// ----- Private methods -----
void Engine::AddTask(lt::add_torrent_params params, error_code & ec) {
    params.save_path = settings_.save_dir;
    if( params.trackers.empty()  && !settings_.trackers.empty() ) {
        for (auto const& tracker : settings_.trackers) {
            params.trackers.push_back(tracker);
        }
    }
    // Add torrent to session
    auto th = session_->add_torrent(std::move(params), ec);
    if(ec) {
        WLOG << "Add BT task error(" << ec.value() 
            << "): " << ec.message();
        return;
    }
}

std::shared_ptr<Engine> create() {
    return std::make_shared<Engine>();
}

} // namespace engine
} // namespace p2pd 
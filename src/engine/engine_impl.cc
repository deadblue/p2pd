#include "engine/engine_impl.h"

#include <fstream>
#include <iomanip>
#include <sstream>

#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/version.hpp>

#include "engine/helper.h"
#include "log/log.h"
#include "version.h"

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

EngineImpl::~EngineImpl() {
    DLOG << "P2PD Engine destroyed!";
}

// ----- Helper functions -----

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

    auto mask = lt::alert_category::error 
        | lt::alert_category::file_progress
        | lt::alert_category::storage
        | lt::alert_category::status;
    settings.set_int(lt::settings_pack::alert_mask, mask);

    return lt::session_params(std::move(settings));
}

// ----- Public API -----

void EngineImpl::Startup(Options const& options) {
    fill_settings(options, settings_);
    // Create session
    auto params = make_session_params(options);
    params.extensions.push_back(shared_from_this());
    session_ = new lt::session(std::move(params));
    // TODO: Load tasks status into database.
}

void EngineImpl::Shutdown() {
    session_->pause();
    // TODO: Save tasks status into database.
    delete session_;
}

void EngineImpl::AddMagnet(const char * uri, error_code & ec) {
    auto params = lt::parse_magnet_uri(uri, ec);
    if(ec) {
        ec = errors::invalid_magnet_uri;
        return;
    }
    AddTaskInternal(std::move(params));
}

void EngineImpl::AddTorrent(
    const uint8_t * data, size_t data_size, error_code & ec
) {
    lt::add_torrent_params params;
    params.ti = std::make_shared<lt::torrent_info>(
        reinterpret_cast<const char*>(data),  data_size, ec
    );
    if(ec) {
        ec = errors::invalid_torrent_data;
        return;
    }
    AddTaskInternal(std::move(params));
}

void EngineImpl::ListTask(std::vector<TaskSummary> & summaries) {
    // Retrieve all handles in session
    auto handles = session_->get_torrents();
    for(auto const& handle : handles) {
        TaskSummary summary;
        if(!fill_task_summary(handle, summary)) { continue; }
        summaries.push_back( std::move(summary) );
    }
}

void EngineImpl::RetrieveTask(
    std::string const& task_id, TaskMetadata & metadata, error_code & ec
) {
    // Decode hash from task_id
    lt::sha1_hash hash;
    if(!decode_hash(task_id, hash)) {
        ec = errors::invalid_task_id;
        return;
    }
    // Retrieve task from session
    auto handle = session_->find_torrent(hash);
    if(!handle.is_valid()) {
        ec = errors::no_such_task;
        return;
    }
    fill_task_metadata(handle, metadata);
}

void EngineImpl::InspectTask(
    std::string const& task_id, TaskStatus & status, error_code & ec
) {
    // Decode hash from task_id
    lt::sha1_hash hash;
    if(!decode_hash(task_id, hash)) {
        ec = errors::invalid_task_id;
        return;
    }
    // Retrieve task from session
    auto handle = session_->find_torrent(hash);
    if(!handle.is_valid()) {
        ec = errors::no_such_task;
        return;
    }

    // Inspect torrent status
    auto s = handle.status(
        lt::torrent_handle::query_pieces
    );
    if(!s.has_metadata) { return; }
    for(auto piece : s.pieces) {
        status.pieces.push_back(piece ? 1 : 0);
    }

    // Inspect peers state
    std::vector<lt::peer_info> pis;
    handle.get_peer_info(pis);
    for(auto const& pi : pis) {
        TaskStatus::Peer peer;
        peer.client = pi.client;
        peer.address = pi.ip.address().to_string();
        peer.dl_bytes = pi.total_download;
        peer.dl_speed = pi.down_speed;
        peer.ul_bytes = pi.total_upload;
        peer.ul_speed = pi.up_speed;
        status.peers.push_back( std::move(peer) );
    }

    // Inspect files progresses
    auto files = handle.file_progress(
        lt::torrent_handle::piece_granularity
    );
    for(auto progress : files) {
        status.files.push_back(progress);
    }
}

// ----- Override |libtorrent::plugin| -----

lt::feature_flags_t EngineImpl::implemented_features() {
    // Receive all alters
    lt::feature_flags_t flags{ lt::plugin::alert_feature };
    return flags;
}

void EngineImpl::on_alert(lt::alert const* alert) {
    switch(alert->type()) {
    case lt::add_torrent_alert::alert_type:
        {
            auto * p = lt::alert_cast<lt::add_torrent_alert>(alert);
            TaskMetadata metadata;
            if(fill_task_metadata(p->handle, metadata)) {
                for(auto const* ob : observers_) {
                    ob->OnTaskCreated(metadata);
                }
            }
        }
        break;
    case lt::metadata_received_alert::alert_type:
        {
            auto * p = lt::alert_cast<lt::metadata_received_alert>(alert);
            TaskMetadata metadata;
            if(fill_task_metadata(p->handle, metadata)) {
                for(auto const* ob : observers_) {
                    ob->OnTaskMetadataReceived(metadata);
                }
            }
        }
        break;
    case lt::state_changed_alert::alert_type:
        {
            auto * p = lt::alert_cast<lt::state_changed_alert>(alert);
            std::string task_id;
            if(encode_hash(p->handle.info_hash(), task_id)) {
                auto old_state = convert_state(p->prev_state);
                auto new_state = convert_state(p->state);
                for(auto const* ob : observers_) {
                    ob->OnTaskStateChanged(task_id, old_state, new_state);
                }
            }
        }
        break;
    case lt::file_completed_alert::alert_type:
        {
            auto * p = lt::alert_cast<lt::file_completed_alert>(alert);
            // TODO: Rename when file completely downloaded.
        }
        break;
    // Ignore alerts
    case lt::external_ip_alert::alert_type:
    case lt::listen_succeeded_alert::alert_type:
        break;
    default:
        LOG << "Alert category=" 
            << static_cast<uint32_t>(alert->category())
            << ", type=" << alert->type();
        break;
    }
}

// ----- Private methods -----

void EngineImpl::AddTaskInternal(lt::add_torrent_params params) {
    params.save_path = settings_.save_dir;
    if( params.trackers.empty()  && !settings_.trackers.empty() ) {
        for (auto const& tracker : settings_.trackers) {
            params.trackers.push_back(tracker);
        }
    }
    if(params.ti) {
        auto files = params.ti->files();
        for(size_t i = 0; i < files.num_files(); ++i) {
            auto file_path = files.file_path(i);
            params.renamed_files[i] = file_path.append(".p2pd");
        }
    }
    session_->async_add_torrent(std::move(params));
}

std::shared_ptr<Engine> create() {
    return std::make_shared<EngineImpl>();
}

} // namespace engine
} // namespace p2pd 
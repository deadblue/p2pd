#include "engine/helper.h"

#include <boost/beast/core/detail/base64.hpp>
#include <libtorrent/torrent_status.hpp>

namespace p2pd {
namespace engine {

bool encode_hash(lt::sha1_hash const& hash, std::string & hash_str) {
    char text[28];
    auto data = reinterpret_cast<const uint8_t *>(hash.data());
    boost::beast::detail::base64::encode(
        text, data, 20
    );
    hash_str.assign(text, 28);
    return true;
}

bool decode_hash(std::string const& hash_str, lt::sha1_hash & hash) {
    if(hash_str.size() != 28) {
        return false;
    }
    char data[20];
    boost::beast::detail::base64::decode(
        data, hash_str.c_str(), 28
    );
    hash.assign(data);
    return true;
}

TaskSummary::State convert_state(lt::torrent_status::state_t state) {
    switch (state) {
    case lt::torrent_status::downloading_metadata:
    case lt::torrent_status::checking_resume_data:
    case lt::torrent_status::checking_files:
        return TaskSummary::preparing;
    case lt::torrent_status::downloading:
        return TaskSummary::downloading;
    case lt::torrent_status::seeding:
        return TaskSummary::uploading;
    case lt::torrent_status::finished:
        return TaskSummary::finished;
    default:
        return TaskSummary::preparing;
    }
}

bool fill_task_metadata(
    lt::torrent_handle const& handle, TaskMetadata & metadata
) {
    if(!handle.is_valid()) { return false; }
    if(!encode_hash(handle.info_hash(), metadata.id)) {
        return false;
    }

    auto status = handle.status(lt::torrent_handle::query_name);
    if(!status.has_metadata) {
        metadata.name = std::move(status.name);
        metadata.ready = false;
    } else {
        auto ti = handle.torrent_file();
        metadata.name = ti->name();
        metadata.creator = ti->creator();
        metadata.comment = ti->comment();
        auto files = ti->files();
        for(size_t i = 0; i < files.num_files(); ++i) {
            metadata.files.emplace_back(
                files.file_path(i), files.file_size(i)
            );
        }
        metadata.ready = true;
    }
    return true;
}

bool fill_task_summary(
    lt::torrent_handle const& handle, TaskSummary & summary
) {
    if(!handle.is_valid()) { return false; }
    // Get task status
    auto status = handle.status(
        lt::torrent_handle::query_name
    );
    encode_hash(handle.info_hash(), summary.id);
    summary.name = std::move(status.name);
    if(!status.has_metadata) {
        summary.state = TaskSummary::preparing;
    } else {
        // Status
        summary.state = convert_state(status.state);
        summary.total_bytes = status.total;
        summary.dl_bytes = status.all_time_download;
        summary.dl_speed = status.download_rate;
        summary.ul_bytes = status.all_time_upload;
        summary.ul_speed = status.upload_rate;
        summary.connected_peers = status.num_peers;
        summary.connected_seeds = status.num_seeds;
    }
    return true;
}

} // namespace engine
} // namespace p2pd
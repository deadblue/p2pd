#ifndef P2PD_ENGINE_HELPER_H
#define P2PD_ENGINE_HELPER_H

#include <string>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/torrent_status.hpp>

#include "engine/type.h"

// namespace libtorrent {
// class sha1_hash;
// class torrent_handle;
// class torrent_info;
// }

namespace p2pd {
namespace engine {

namespace lt = libtorrent;

bool encode_hash(lt::sha1_hash const& hash, std::string & hash_str);
bool decode_hash(std::string const& hash_str, lt::sha1_hash & hash);

/**
 * @brief Convert libtorrent state enum into engine enum.
 */
TaskSummary::State convert_state(lt::torrent_status::state_t state);

/**
 * @brief Fill task metadata from torrent info.
 */
bool fill_task_metadata(
    lt::torrent_handle const& handle, TaskMetadata & metadata
);

/**
 * @brief Fill Task information which retrieved from |lt::torrent_handle|.
 * 
 * @return false when handle is invalid, true otherwise.
 */
bool fill_task_summary(
    lt::torrent_handle const& handle, TaskSummary & summary
);

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_HELPER_H
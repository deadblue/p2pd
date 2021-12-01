#ifndef P2PD_ENGINE_TYPE_H
#define P2PD_ENGINE_TYPE_H

#include <cstdint>
#include <string>
#include <vector>

namespace p2pd {
namespace engine {

/**
 * @brief   Task metadata information which retrieved from torrent, it is 
 *          immutable, so client can cache it on client side.
 */
struct TaskMetadata {

    /**
     * @brief   File information.
     */
    struct File {
        // File path
        std::string path;
        // File size
        uint64_t size;

        // Contructor
        File() = default;
        // Contructor with all fields.
        File(std::string path, uint64_t size) 
            : path(std::move(path)), size(size) {}
    };

    // Task ID
    std::string id;
    // Torrent name
    std::string name;
    // Torrent creator
    std::string creator;
    // Torrent comment
    std::string comment;
    // File list
    std::vector<File> files;
    // Ready flag, when false, only id and name are available.
    bool ready = false;
};

/**
 * @brief   Task summary information for showing in list.
 */
struct TaskSummary {

    // Task running state.
    enum State {
        preparing = 0,
        downloading,
        uploading,
        finished,
        paused
    };

    // Unique task ID
    std::string id;
    // Task name
    std::string name;
    // Task state
    State state;

    // Total bytes that will be downloaded
    uint64_t total_bytes;
    // Downloaded bytes
    uint64_t dl_bytes;
    // Download bytes per seconds.
    uint64_t dl_speed;
    // Uploaded bytes
    uint64_t ul_bytes;
    // Upload bytes per seconds.
    uint64_t ul_speed;
    // The number of currently connected peers.
    int connected_peers;
    // The number of currently connected seeds.
    int connected_seeds;
};

/**
 * @brief   The detail status of a task.
 */
struct TaskStatus {

    struct Peer {
        // Client name
        std::string client;
        // Peer address, in form of "IP:Port"
        std::string address;
        // Total downloaded bytes from this peer.
        uint64_t dl_bytes;
        // Current download speed from this peer.
        uint64_t dl_speed;
        // Total updated bytes to this peer;
        uint64_t ul_bytes;
        // Current upload speed to this peer.
        uint64_t ul_speed;
    };

    // Pieces status
    std::vector<int> pieces;
    // Files progress
    std::vector<uint64_t> files;
    // Peers status.
    std::vector<Peer> peers;
};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_TYPE_H
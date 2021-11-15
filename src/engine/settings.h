#ifndef P2PD_ENGINE_SETTINGS_H
#define P2PD_ENGINE_SETTINGS_H

#include <string>
#include <vector>

namespace p2pd {
namespace engine {

struct Settings {
    // Directory for store downloading files.
    std::string save_dir;
    // Tracker list
    std::vector<std::string> trackers;
};

} // namespace engine
} // namespace p2pd

#endif // P2PD_ENGINE_SETTINGS_H
